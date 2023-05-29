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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_SIGNALHELPERS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_SIGNALHELPERS_H_

#include <ecu/com/message.h>
#include <ecu/rt/signaladapter.h>

#include <list>
#include <type_traits>

namespace test_util {

class SignalInfo {
 public:
  using ByteType = ecu::lapi::rt::Signal::BYTE;
  using DataType = ecu::lapi::rt::Signal::DATA;
  using ValueType = double;

  SignalInfo(const char* name, std::initializer_list<ByteType> data);

  SignalInfo(const char* name, const DataType& data);

  SignalInfo(const char* name, const ValueType& value);

  template<class T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
  SignalInfo(const char* name, T value) : SignalInfo(name, &value, sizeof(value)) {}

  SignalInfo(const char* name, const void* data, std::size_t size);

  const char* GetName() const;

  bool SetSignal(ecu::lapi::rt::SignalGroup* signal_group) const;

 private:
  const char* name_;
  DataType data_;
  ValueType value_;
};

using SignalInfoList = std::list<SignalInfo>;

std::unique_ptr<ecu::lapi::rt::SignalGroup> DecodeSignalGroup(
    const std::string& topic, const ecu::lapi::com::Message& message);

ecu::lapi::rt::SignalGroup GenerateSignalGroup(
    const char* topic, const SignalInfoList& signals);

ecu::lapi::com::Message GenerateSignalGroupMessage(
    const char* topic, const SignalInfoList& signals);

ecu::lapi::com::Message PackSignalGroup(
    const ecu::lapi::rt::SignalGroup& signal_group);

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_SIGNALHELPERS_H_

/************************************************************************************/
