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

/*
**====================================================================================
** Provided include files
**====================================================================================
*/
#include "util/SignalGroupObserver.h"

/*
**====================================================================================
** Required include files
**====================================================================================
*/
#include <ecu/rt/signaladapter.h>
#include <ecu/logging.h>

#include <cassert>

#include <memory>
#include <utility>

/*
**====================================================================================
** Named Namespaces
**====================================================================================
*/
namespace util {

SignalGroupObserver::SignalGroupObserver(CallbackFunc&& callback)
    : callback_(std::move(callback)) {
  assert(callback_);
}

void SignalGroupObserver::message(const std::string& topic,
                                  const ecu::lapi::com::Message& message) {
  auto& adapter = ecu::lapi::rt::SignalAdapter::instance();
  auto result = adapter.unpack(topic, message);
  if (result.ok()) {
    callback_(std::move(result.val()));
  } else {
    LOG(ERROR) << "Deserialize error [" << result.err_val().code << "] for topic: " << topic;
  }
}

ecu::lapi::com::ISubscriptionObserver_ptr CreateSignalGroupObserver(
    SignalGroupObserver::CallbackFunc&& callback) {
  return std::make_shared<SignalGroupObserver>(std::move(callback));
}

}  // namespace util

/************************************************************************************/
