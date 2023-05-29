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

#include "mqtt_util/BackendProxy.h"

#include <ecu/com/backend.h>

#include <chrono>  // NOLINT
#include <utility>

namespace mqtt_util {

using BackendClient = ecu::lapi::com::BackendClient;

BackendProxy::BackendProxy(Client* client)
    : client_(client) {
  assert(client_);

  client_->Subscribe(BackendClient::TOPIC_PUB);
}

BackendProxy::~BackendProxy() {
  client_->Unsubscribe(BackendClient::TOPIC_PUB);
}

void BackendProxy::PublishConnectionState(
    ecu::lapi::pb::BackendProxyStatus_ConnectionState con_state) const {
  auto status_message = ecu::lapi::pb::BackendProxyStatus();
  status_message.set_con_state(con_state);

  client_->PublishMessage(BackendClient::TOPIC_STATUS, status_message);
}

BackendProxy::TransmitMessagePtr BackendProxy::GetTransmitMessage(int timeout) {
  if (!message_queue_.empty()) {
    auto message = std::move(message_queue_.front());
    message_queue_.pop_front();
    return message;
  }

  return client_->GetMessage<TransmitMessagePtr::element_type>(
      BackendClient::TOPIC_PUB, timeout);
}

BackendProxy::TransmitMessagePtr BackendProxy::GetTransmitMessage(
    std::uint32_t content_id, int timeout) {
  for (auto it = message_queue_.begin(); it != message_queue_.end(); ++it) {
    assert(*it);

    if ((*it)->content_id() == content_id) {
      auto message = std::move(*it);
      message_queue_.erase(it);
      return message;
    }
  }

  auto remaining = std::chrono::milliseconds(timeout);
  auto deadline = std::chrono::steady_clock::now() + remaining;

  while (remaining >= remaining.zero()) {
    auto message = client_->GetMessage<TransmitMessagePtr::element_type>(
        BackendClient::TOPIC_PUB, remaining.count());
    if (!message) {
      break;
    }

    if (message->content_id() == content_id) {
      return message;
    }

    message_queue_.push_back(std::move(message));

    remaining = std::chrono::duration_cast<decltype(remaining)>(
        deadline - decltype(deadline)::clock::now());
  }

  return nullptr;
}

BackendProxy::RespondFunc BackendProxy::GetRespondFunc(
    const TransmitMessagePtr& message) const {
  if (message && message->req_response()) {
    auto id = message->req_id();
    auto topic = "app/" + message->req_client() + "/be/response";

    return [id, topic, this](ecu::lapi::pb::BackendProxyResponse_Status status) {
      auto response_message = ecu::lapi::pb::BackendProxyResponse();
      response_message.set_req_id(id);
      response_message.set_status(status);

      client_->PublishMessage(topic.c_str(), response_message);
    };
  }

  return nullptr;
}

std::string BackendProxy::GetPublishTopic(std::uint32_t content_id) const {
  return BackendClient::TOPIC_CONTENT + std::to_string(content_id);
}

template<>
std::unique_ptr<std::string> BackendProxy::DeserializeMessage(
    const TransmitMessagePtr& message) {
  if (message) {
    return std::unique_ptr<std::string>(message->release_payload());
  }

  return nullptr;
}

}  // namespace mqtt_util

/************************************************************************************/
