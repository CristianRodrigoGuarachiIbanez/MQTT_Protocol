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

#ifndef MQTT_UTIL_INCLUDE_MQTT_UTIL_CLIENT_H_
#define MQTT_UTIL_INCLUDE_MQTT_UTIL_CLIENT_H_

#include <ecu/com/message.h>
#include <ecu/pb/message.pb.h>
#include <ecu/rt/signaladapter.h>
#include <mosquittopp.h>

#include <condition_variable>  // NOLINT
#include <deque>
#include <map>
#include <memory>
#include <mutex>  // NOLINT
#include <set>
#include <string>

#include "test_util/MessageHelpers.h"
#include "test_util/SignalHelpers.h"

namespace mqtt_util {

class Client : protected mosqpp::mosquittopp {
 public:
  using LapiMessagePtr = std::unique_ptr<ecu::lapi::com::Message>;
  using SignalGroupPtr = std::unique_ptr<ecu::lapi::rt::SignalGroup>;

  Client(const char* host, int port, const char* id = nullptr);

  Client(const char* host, const char* port, const char* id = nullptr);

  ~Client();

  void Subscribe(const char* topic);

  void Unsubscribe(const char* topic);

  void Unretain(const char* topic);

  LapiMessagePtr GetLapiMessage(const char* topic, int timeout);

  SignalGroupPtr GetSignalGroup(const char* topic, int timeout);

  template<class T>
  std::unique_ptr<T> GetMessage(const char* topic, int timeout) {
    auto lapi_message = GetLapiMessage(topic, timeout);
    return lapi_message ? ::test_util::DecodeMessage<T>(*lapi_message) : nullptr;
  }

  void PublishMessage(const char* topic, const ecu::lapi::com::Message& message);

  template<class T>
  void PublishMessage(const char* topic, const T& message, bool retain = false) {
    auto lapi_message = ::test_util::GenerateMessage(message);
    lapi_message.set_retain(retain);

    PublishMessage(topic, lapi_message);
  }

  void PublishSignal(const char* topic, const char* name,
                     std::initializer_list<::test_util::SignalInfo::ByteType> data,
                     bool retain = false);

  void PublishSignal(const char* topic, const char* name,
                     const ::test_util::SignalInfo::DataType& data,
                     bool retain = false);

  void PublishSignal(const char* topic, const char* name,
                     const ::test_util::SignalInfo::ValueType& value,
                     bool retain = false);

  void PublishSignalGroup(const char* topic, const ::test_util::SignalInfoList& signals,
                          bool retain = false);

 private:
  using MessageQueue = std::deque<LapiMessagePtr>;

  void NotifyMessageId(int mid);

  void WaitForMessageId(int mid, int timeout);

  void on_publish(int mid) final;

  void on_message(const mosquitto_message* message) final;

  void on_subscribe(int mid, int qos_count, const int* granted_qos) final;

  void on_unsubscribe(int mid) final;

  void on_log(int level, const char* str) final;

  std::map<std::string, MessageQueue> messages_;
  std::set<int> message_ids_;

  std::condition_variable cond_var_;
  std::mutex mutex_;

  ecu::lapi::pb::SubscriptionStatus subscription_status_;
};

}  // namespace mqtt_util

#endif  // MQTT_UTIL_INCLUDE_MQTT_UTIL_CLIENT_H_

/************************************************************************************/
