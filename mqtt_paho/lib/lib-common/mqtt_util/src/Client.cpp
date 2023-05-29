/*
**************************************************************************************
** Project: TBM/VCM Platform
**
** Copyright (C) MAN Truck & Bus AG
**
** This document contains proprietary information belonging to MAN Truck & Bus AG.
** Passing on and copying of this document, and communication of its contents
** is not permitted without prior written authorization.
**
**************************************************************************************
*/

#include "mqtt_util/Client.h"

#include <ecu/com/client.h>
#include <ecu/com/message.h>

#include <chrono>  // NOLINT
#include <cstring>
#include <iostream>
#include <utility>

namespace {

const auto mqtt_client_debug = std::getenv("MQTT_CLIENT_DEBUG");

std::runtime_error GenerateMqttError(const std::string& cause, int errnum = 0) {
  auto description = std::string("MQTT: ").append(cause);
  if (errnum) {
    description.append(": ").append(mosqpp::strerror(errnum));
  }

  return std::runtime_error(description);
}

std::string GetLapiTopic(const char* topic) {
  assert(topic);

  if (std::strncmp(topic, "rt/", 3) == 0) {
    return std::string("rttx/") + (topic + 3);
  }

  return topic;
}

}  // namespace

namespace mqtt_util {

Client::Client(const char* host, int port, const char* id)
    : mosquittopp(id) {
  if (!host) {
    throw GenerateMqttError("Host is not specified");
  }

  auto result = connect(host, port);
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("connect() returned error", result);
  }

  result = loop_start();
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("loop_start() returned error", result);
  }

  Subscribe(ecu::lapi::com::ANNOUNCEMENT_TOPIC.c_str());
}

Client::Client(const char* host, const char* port, const char* id)
    : Client(host, std::stoi(port), id) {}

Client::~Client() {
  Unretain(ecu::lapi::com::IPC_SUBSCRIPTIONS_TOPIC.c_str());

  disconnect();
  loop_stop();
}

void Client::Subscribe(const char* topic) {
  auto lapi_topic = GetLapiTopic(topic);

  {
    auto lock = std::unique_lock<decltype(mutex_)>(mutex_);
    messages_.emplace(lapi_topic, MessageQueue());
  }

  auto mid = 0;
  auto result = subscribe(&mid, lapi_topic.c_str(), 0);
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("subscribe() returned error", result);
  }

  WaitForMessageId(mid, 2000);
}

void Client::Unsubscribe(const char* topic) {
  auto lapi_topic = GetLapiTopic(topic);

  {
    auto lock = std::unique_lock<decltype(mutex_)>(mutex_);
    messages_.erase(lapi_topic);
  }

  auto mid = 0;
  auto result = unsubscribe(&mid, lapi_topic.c_str());
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("unsubscribe() returned error", result);
  }

  WaitForMessageId(mid, 2000);
}

void Client::Unretain(const char* topic) {
  assert(topic);

  auto mid = 0;
  auto result = publish(&mid, topic, 0, nullptr, 1, true);
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("publish() returned error", result);
  }

  WaitForMessageId(mid, 2000);
}

Client::LapiMessagePtr Client::GetLapiMessage(const char* topic, int timeout) {
  auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
  auto lapi_topic = GetLapiTopic(topic);
  auto lock = std::unique_lock<decltype(mutex_)>(mutex_);

  auto it = messages_.find(lapi_topic);
  if (it == messages_.end()) {
    return {};
  }

  auto& queue = it->second;
  do {
    if (!queue.empty()) {
      auto result = std::move(queue.front());
      queue.pop_front();
      return result;
    }
  } while (cond_var_.wait_until(lock, deadline) == std::cv_status::no_timeout);

  return {};
}

Client::SignalGroupPtr Client::GetSignalGroup(const char* topic, int timeout) {
  assert(topic);

  auto lapi_message = GetLapiMessage(topic, timeout);
  if (!lapi_message) {
    return {};
  }

  auto result = ecu::lapi::rt::SignalAdapter::instance().unpack(topic, *lapi_message);
  if (result.nok()) {
    return {};
  }

  using SignalGroupType = SignalGroupPtr::element_type;

  return SignalGroupPtr(new SignalGroupType(std::move(result.val())));
}

void Client::PublishMessage(const char* topic, const ecu::lapi::com::Message& message) {
  assert(topic);

  auto& buffer = message.get_buffer();
  auto mid = 0;
  auto result = publish(&mid, topic, buffer.size(), buffer.data(), 1, message.get_retain());
  if (result != MOSQ_ERR_SUCCESS) {
    throw GenerateMqttError("publish() returned error", result);
  }

  WaitForMessageId(mid, 2000);
}

void Client::PublishSignal(const char* topic, const char* name,
                           std::initializer_list<::test_util::SignalInfo::ByteType> data,
                           bool retain) {
  PublishSignalGroup(topic, { {name, data} }, retain);
}

void Client::PublishSignal(const char* topic, const char* name,
                           const ::test_util::SignalInfo::DataType& data,
                           bool retain) {
  PublishSignalGroup(topic, { {name, data} }, retain);
}

void Client::PublishSignal(const char* topic, const char* name,
                           const ::test_util::SignalInfo::ValueType& value,
                           bool retain) {
  PublishSignalGroup(topic, { {name, value} }, retain);
}

void Client::PublishSignalGroup(const char* topic, const ::test_util::SignalInfoList& signals,
                                bool retain) {
  auto message = ::test_util::GenerateSignalGroupMessage(topic, signals);
  message.set_retain(retain);

  PublishMessage(topic, message);
}

void Client::NotifyMessageId(int mid) {
  auto lock = std::unique_lock<decltype(mutex_)>(mutex_);

  if (message_ids_.emplace(mid).second) {
    cond_var_.notify_all();
  } else {
    throw GenerateMqttError("Message ID already exists");
  }
}

void Client::WaitForMessageId(int mid, int timeout) {
  auto lock = std::unique_lock<decltype(mutex_)>(mutex_);

  if (!cond_var_.wait_for(lock, std::chrono::milliseconds(timeout), [&]() {
    auto it = message_ids_.find(mid);
    if (it == message_ids_.end()) {
      return false;
    }

    message_ids_.erase(it);
    return true;
  })) {
    throw GenerateMqttError("Message ID not received in time");
  }
}

void Client::on_publish(int mid) {
  NotifyMessageId(mid);
}

void Client::on_message(const mosquitto_message* mqtt_message) {
  assert(mqtt_message);
  assert(mqtt_message->topic);

  using LapiMessage = LapiMessagePtr::element_type;
  using DataBuffer = LapiMessage::DataBuffer;

  auto payload = static_cast<DataBuffer::pointer>(mqtt_message->payload);
  auto buffer = DataBuffer(payload, payload + mqtt_message->payloadlen);
  auto lapi_message = LapiMessagePtr(new LapiMessage(std::move(buffer)));
  lapi_message->set_qos(mqtt_message->qos);
  lapi_message->set_retain(mqtt_message->retain);

  if (mqtt_message->topic == ecu::lapi::com::ANNOUNCEMENT_TOPIC) {
    auto sub_message = ::test_util::DecodeMessage<ecu::lapi::pb::SubscriptionMessage>(
        *lapi_message);
    if (!sub_message) {
      throw GenerateMqttError("Unable to decode SubscriptionMessage");
    }

    for (auto& topic : sub_message->topics()) {
      subscription_status_.add_topics(topic);
    }

    auto sub_status = ::test_util::GenerateMessage(subscription_status_);
    auto result = publish(nullptr, ecu::lapi::com::IPC_SUBSCRIPTIONS_TOPIC.c_str(),
                          sub_status.get_buffer().size(),
                          sub_status.get_buffer().data(),
                          0, true);
    if (result != MOSQ_ERR_SUCCESS) {
      throw GenerateMqttError("publish() returned error", result);
    }
  }

  auto lock = std::unique_lock<decltype(mutex_)>(mutex_);

  auto it = messages_.find(mqtt_message->topic);
  if (it != messages_.end()) {
    it->second.emplace_back(std::move(lapi_message));
    cond_var_.notify_all();
  }
}

void Client::on_subscribe(int mid, int, const int*) {
  NotifyMessageId(mid);
}

void Client::on_unsubscribe(int mid) {
  NotifyMessageId(mid);
}

void Client::on_log(int, const char* str) {
  if (mqtt_client_debug) {
    std::cout << "MQTT: " << str << std::endl;
  }
}

}  // namespace mqtt_util

/************************************************************************************/
