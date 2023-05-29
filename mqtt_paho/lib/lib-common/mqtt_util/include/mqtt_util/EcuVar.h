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

#ifndef MQTT_UTIL_INCLUDE_MQTT_UTIL_ECUVAR_H_
#define MQTT_UTIL_INCLUDE_MQTT_UTIL_ECUVAR_H_

#include <string>

#include "Client.h"

namespace mqtt_util {

class EcuVar {
 public:
  explicit EcuVar(Client* client);

  void Set(const char* variable, const char* value, bool retain = false) const;

  void Unretain(const char* variable) const;

  static std::string GetVariableTopic(const char* variable);

 private:
  Client* client_;
};

}  // namespace mqtt_util

#endif  // MQTT_UTIL_INCLUDE_MQTT_UTIL_ECUVAR_H_

/************************************************************************************/
