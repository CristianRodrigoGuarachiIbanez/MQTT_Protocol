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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_MESSAGEHELPERS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_MESSAGEHELPERS_H_

#include <google/protobuf/message.h>
#include <ecu/com/message.h>
#include <ecu/com/messageadapter.h>

#include <memory>
#include <type_traits>
#include <utility>

namespace test_util {

template<class T>
std::unique_ptr<T> DecodeBackendMessage(const void* data, int size) {
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T is not a protobuf message");

  auto protobuf = std::unique_ptr<T>(new T);
  if (protobuf->ParseFromArray(data, size)) {
    return protobuf;
  }

  return nullptr;
}

template<class T>
std::unique_ptr<T> DecodeBackendMessage(const ecu::lapi::com::Message& message) {
  auto& buffer = message.get_buffer();
  return DecodeBackendMessage<T>(buffer.data(), buffer.size());
}

template<class T>
std::unique_ptr<T> DecodeMessage(const ecu::lapi::com::Message& message) {
  auto adapter = ecu::lapi::com::PbInternalMessageAdapter<T>();
  auto result = adapter.deserialize(message);
  if (result.ok()) {
    return std::unique_ptr<T>(new T(std::move(result.val())));
  }

  return nullptr;
}

template<class T>
ecu::lapi::com::Message GenerateMessage(const T& message) {
  auto adapter = ecu::lapi::com::PbInternalMessageAdapter<T>();
  auto result = adapter.serialize(message);
  assert(result.ok());

  return std::move(result.val());
}

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_MESSAGEHELPERS_H_

/************************************************************************************/
