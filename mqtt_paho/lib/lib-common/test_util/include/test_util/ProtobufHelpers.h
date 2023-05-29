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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_PROTOBUFHELPERS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_PROTOBUFHELPERS_H_

#include <google/protobuf/message.h>

#include <ostream>
#include <type_traits>

inline bool operator==(const ::google::protobuf::MessageLite& msg1,
                       const ::google::protobuf::MessageLite& msg2) {
  return ((msg1.GetTypeName() == msg2.GetTypeName()) &&
          (msg1.SerializeAsString() == msg2.SerializeAsString()));
}

inline bool operator!=(const ::google::protobuf::MessageLite& msg1,
                       const ::google::protobuf::MessageLite& msg2) {
  return !(msg1 == msg2);
}

template<class T, class = std::enable_if_t<std::is_base_of<::google::protobuf::Message, T>::value>>
std::ostream& operator<<(std::ostream& os, const T& msg) {
  return os << msg.ShortDebugString();
}

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_PROTOBUFHELPERS_H_

/************************************************************************************/
