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

#ifndef MQTT_UTIL_INCLUDE_MQTT_UTIL_BACKENDPROXY_H_
#define MQTT_UTIL_INCLUDE_MQTT_UTIL_BACKENDPROXY_H_

#include <ecu/pb/backend.pb.h>

#include <deque>
#include <memory>
#include <string>
#include <utility>

#include "test_util/MessageHelpers.h"

#include "Client.h"

namespace mqtt_util {

class BackendProxy {
 public:
  using RespondFunc = std::function<void(ecu::lapi::pb::BackendProxyResponse_Status)>;

  explicit BackendProxy(Client* client);

  BackendProxy(const BackendProxy&) = delete;

  BackendProxy(BackendProxy&&) = default;

  ~BackendProxy();

  template<class T>
  std::unique_ptr<T> GetMessage(int timeout) {
    return DeserializeMessage<T>(GetTransmitMessage(timeout));
  }

  template<class T>
  std::unique_ptr<T> GetMessage(std::uint32_t content_id, int timeout) {
    return DeserializeMessage<T>(GetTransmitMessage(content_id, timeout));
  }

  template<class T>
  std::pair<std::unique_ptr<T>, RespondFunc> GetMessageWithResponse(int timeout) {
    auto message = GetTransmitMessage(timeout);
    return std::make_pair(DeserializeMessage<T>(message), GetRespondFunc(message));
  }

  template<class T>
  std::pair<std::unique_ptr<T>, RespondFunc> GetMessageWithResponse(
      std::uint32_t content_id, int timeout) {
    auto message = GetTransmitMessage(content_id, timeout);
    return std::make_pair(DeserializeMessage<T>(message), GetRespondFunc(message));
  }

  template<class T>
  void PublishMessage(std::uint32_t content_id, const T& message) {
    client_->PublishMessage(GetPublishTopic(content_id).c_str(), message);
  }

  void PublishConnectionState(
      ecu::lapi::pb::BackendProxyStatus_ConnectionState con_state) const;

 protected:
  using TransmitMessagePtr = std::unique_ptr<ecu::lapi::pb::BackendProxyTransmit>;

  template<class T>
  std::unique_ptr<T> DeserializeMessage(const TransmitMessagePtr& message) {
    if (message && message->has_payload()) {
      auto& payload = message->payload();
      return ::test_util::DecodeBackendMessage<T>(payload.data(), payload.size());
    }

    return nullptr;
  }

  TransmitMessagePtr GetTransmitMessage(int timeout);

  TransmitMessagePtr GetTransmitMessage(std::uint32_t content_id, int timeout);

  RespondFunc GetRespondFunc(const TransmitMessagePtr& message) const;

  std::string GetPublishTopic(std::uint32_t content_id) const;

 private:
  Client* client_;
  std::deque<TransmitMessagePtr> message_queue_;
};

template<>
std::unique_ptr<std::string> BackendProxy::DeserializeMessage(
    const TransmitMessagePtr& message);

}  // namespace mqtt_util

#endif  // MQTT_UTIL_INCLUDE_MQTT_UTIL_BACKENDPROXY_H_

/************************************************************************************/
