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

#ifndef UTIL_INCLUDE_UTIL_MESSAGEOBSERVER_H_
#define UTIL_INCLUDE_UTIL_MESSAGEOBSERVER_H_

#include <ecu/com/messageadapter.h>
#include <ecu/com/observer.h>
#include <ecu/logging.h>

#include <cassert>

#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace util {

template<class T>
class MessageObserver : public ecu::lapi::com::ISubscriptionObserver {
 public:
  using CallbackFunc = std::function<void(T&&)>;

  explicit MessageObserver(CallbackFunc&& callback) : callback_(std::move(callback)) {
    assert(callback_);
  }

 private:
  void message(const std::string& topic, const ecu::lapi::com::Message& message) override {
    auto adapter = ecu::lapi::com::PbInternalMessageAdapter<T>();
    auto result = adapter.deserialize(message);
    if (result.ok()) {
      callback_(std::move(result.val()));
    } else {
      LOG(ERROR) << "Deserialize error [" << result.err_val() << "] for topic: " << topic;
    }
  }

  CallbackFunc callback_;
};

template<class T>
ecu::lapi::com::ISubscriptionObserver_ptr CreateMessageObserver(
    typename MessageObserver<T>::CallbackFunc&& callback) {
  return std::make_shared<MessageObserver<T>>(std::move(callback));
}

}  // namespace util

#endif  // UTIL_INCLUDE_UTIL_MESSAGEOBSERVER_H_

/************************************************************************************/
