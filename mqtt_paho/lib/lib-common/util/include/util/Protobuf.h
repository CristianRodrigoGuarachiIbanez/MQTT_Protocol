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

#ifndef UTIL_INCLUDE_UTIL_PROTOBUF_H_
#define UTIL_INCLUDE_UTIL_PROTOBUF_H_

#include <google/protobuf/message.h>

#include <fstream>
#include <memory>
#include <string>
#include <type_traits>

namespace util {

/**
** \brief Reads protobuf message from file.
**
** \tparam T Type of protobuf message.
**
** \param[in] path Path to file.
**
** \return Pointer to protobuf message if message was successfully read from file or
**         \c nullptr in case of failure.
*/
template<class T>
std::unique_ptr<T> ReadProtobufMessage(const char* path) {
  static_assert(std::is_base_of<::google::protobuf::Message, T>::value,
                "T is not a protobuf message");

  std::ifstream file(path);
  if (file) {
    auto message = std::unique_ptr<T>(new T);
    if (message->ParseFromIstream(&file)) {
      return message;
    }
  }

  return nullptr;
}

/**
** \brief Reads protobuf message from file.
**
** \param[in] output Protobuf message.
** \param[in] path   Path to file.
**
** \return True if protobuf messages was successfully read from file.
*/
bool ReadProtobufMessage(::google::protobuf::Message* output,
                         const char* path);

/**
** \brief Writes protobuf message to file.
**
** \param[in] message Protobuf message.
** \param[in] path    Path to file.
**
** \return True if protobuf messages was successfully written to file.
*/
bool WriteProtobufMessage(const ::google::protobuf::Message& message,
                          const char* path);

/**
** \brief Converts protobuf message to debug string.
**
** \details Format of string depends on application log level.
**
** \param[in] message Protobuf message.
**
** \return String with human readable debug form of the protobuf message.
*/
std::string ToString(const ::google::protobuf::Message& message);

}  // namespace util

#endif  // UTIL_INCLUDE_UTIL_PROTOBUF_H_

/************************************************************************************/
