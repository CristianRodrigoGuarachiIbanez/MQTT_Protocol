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

#include "mqtt_util/EcuVar.h"

#include <ecu/pb/variables.pb.h>
#include <ecu/variables.h>

#include <chrono>  // NOLINT

namespace mqtt_util {

EcuVar::EcuVar(Client* client)
    : client_(client) {
  assert(client_);
}

void EcuVar::Set(const char* variable, const char* value, bool retain) const {
  assert(variable);
  assert(value);

  auto now = std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();

  auto ecu_variable = ecu::lapi::pb::EcuVariable();
  ecu_variable.set_value(value);
  ecu_variable.set_timestamp(now);

  client_->PublishMessage(GetVariableTopic(variable).c_str(), ecu_variable, retain);
}

void EcuVar::Unretain(const char* variable) const {
  assert(variable);

  client_->Unretain(GetVariableTopic(variable).c_str());
}

std::string EcuVar::GetVariableTopic(const char* variable) {
  assert(variable);

  return std::string(ecu::lapi::var::VariablesClient::TOPIC_PUB) + variable;
}

}  // namespace mqtt_util

/************************************************************************************/
