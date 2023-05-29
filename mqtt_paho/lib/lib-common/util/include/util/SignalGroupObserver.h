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

#ifndef UTIL_INCLUDE_UTIL_SIGNALGROUPOBSERVER_H_
#define UTIL_INCLUDE_UTIL_SIGNALGROUPOBSERVER_H_

#include <ecu/com/observer.h>
#include <ecu/rt/signaladapter.h>

#include <functional>
#include <string>

namespace util {

class SignalGroupObserver : public ecu::lapi::com::ISubscriptionObserver {
 public:
  using CallbackFunc = std::function<void(ecu::lapi::rt::SignalGroup&&)>;

  explicit SignalGroupObserver(CallbackFunc&& callback);

 private:
  void message(const std::string& topic, const ecu::lapi::com::Message& message) override;

  CallbackFunc callback_;
};

ecu::lapi::com::ISubscriptionObserver_ptr CreateSignalGroupObserver(
    SignalGroupObserver::CallbackFunc&& callback);

}  // namespace util

#endif  // UTIL_INCLUDE_UTIL_SIGNALGROUPOBSERVER_H_

/************************************************************************************/
