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

#include "test_util/SignalHelpers.h"

#include <cassert>

#include <stdexcept>
#include <string>
#include <utility>

namespace test_util {

SignalInfo::SignalInfo(const char* name, std::initializer_list<ByteType> data)
    : SignalInfo(name, data.begin(), data.size()) {}

SignalInfo::SignalInfo(const char* name, const DataType& data)
    : name_(name),
      data_(data),
      value_() {
  assert(name_);
}

SignalInfo::SignalInfo(const char* name, const ValueType& value)
    : name_(name),
      data_(),
      value_(value) {
  assert(name_);
}

SignalInfo::SignalInfo(const char* name, const void* data, std::size_t size)
    : name_(name),
      data_(static_cast<const ByteType*>(data), static_cast<const ByteType*>(data) + size),
      value_() {
  assert(name_);
}

const char* SignalInfo::GetName() const {
  return name_;
}

bool SignalInfo::SetSignal(ecu::lapi::rt::SignalGroup* signal_group) const {
  assert(signal_group);

  return data_.empty() ?
      signal_group->set_signal_scaled_value(name_, value_) :
      signal_group->set_signal_data(name_, data_);
}

std::unique_ptr<ecu::lapi::rt::SignalGroup> DecodeSignalGroup(
    const std::string& topic, const ecu::lapi::com::Message& message) {
  auto unpack_result = ecu::lapi::rt::SignalAdapter::instance().unpack(topic, message);
  if (unpack_result.ok()) {
    return std::make_unique<ecu::lapi::rt::SignalGroup>(std::move(unpack_result.val()));
  }

  return nullptr;
}

ecu::lapi::rt::SignalGroup GenerateSignalGroup(
    const char* topic, const SignalInfoList& signals) {
  assert(topic);

  auto create_sg_result = ecu::lapi::rt::SignalAdapter::instance().create_sg(topic);
  if (create_sg_result.nok()) {
    throw std::runtime_error(std::string() +
        "Unable to create SignalGroup for topic " + topic);
  }

  auto& signal_group = create_sg_result.val();
  for (auto& signal : signals) {
    if (!signal.SetSignal(&signal_group)) {
      throw std::runtime_error(std::string() +
          "Unable to set signal " + signal.GetName() + " within SignalGroup for topic " + topic);
    }
  }

  return signal_group;
}

ecu::lapi::com::Message GenerateSignalGroupMessage(
    const char* topic, const SignalInfoList& signals) {
  auto signal_group = GenerateSignalGroup(topic, signals);

  return PackSignalGroup(signal_group);
}

ecu::lapi::com::Message PackSignalGroup(
    const ecu::lapi::rt::SignalGroup& signal_group) {
  auto pack_result = ecu::lapi::rt::SignalAdapter::instance().pack(signal_group);
  if (pack_result.nok()) {
    throw std::runtime_error(std::string() +
        "Unable to pack SignalGroup for topic " + signal_group.topic());
  }

  return pack_result.val();
}

}  // namespace test_util

/************************************************************************************/
