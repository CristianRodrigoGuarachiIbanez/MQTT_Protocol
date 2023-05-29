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
#include "util/Protobuf.h"

/*
**====================================================================================
** Required include files
**====================================================================================
*/
#include <ecu/logging.h>

/*
**====================================================================================
** Named Namespaces
**====================================================================================
*/
namespace util {

bool ReadProtobufMessage(::google::protobuf::Message* output,
                         const char* path) {
  assert(output);

  std::ifstream file(path);
  if (file && output->ParseFromIstream(&file)) {
    return true;
  }

  return false;
}

bool WriteProtobufMessage(const ::google::protobuf::Message& message,
                          const char* path) {
  std::ofstream file(path);
  return (file && message.SerializeToOstream(&file));
}

std::string ToString(const ::google::protobuf::Message& message) {
  if (ecu::lapi::logging::get_log_level() != LOG_DEBUG) {
    return message.GetTypeName() + ": { " + message.ShortDebugString() + " }";
  } else {
    return message.GetTypeName() + ":\n" + message.DebugString();
  }
}

}  // namespace util

/************************************************************************************/
