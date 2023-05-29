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

#ifndef MQTT_UTIL_INCLUDE_MQTT_UTIL_UDSINTERFACE_H_
#define MQTT_UTIL_INCLUDE_MQTT_UTIL_UDSINTERFACE_H_

#include <ecu/pb/uds.pb.h>

#include <string>

#include "util/Version.h"
#include "Client.h"

namespace mqtt_util {

struct UdsContext {
#if SDK_VERSION_CODE >= GET_SDK_VERSION(7, 7, 0)
  using TransportProtocol = ecu::lapi::pb::UdsInterface_TransportProtocol;
  using PhysicalInterface = ecu::lapi::pb::UdsInterface_PhysicalInterface;
#endif

  std::string client_name;

#if SDK_VERSION_CODE >= GET_SDK_VERSION(7, 7, 0)
  TransportProtocol transport_protocol;
  PhysicalInterface physical_interface;
#endif

  std::uint8_t source_address;
  std::uint8_t target_address;
};

class UdsInterface {
 public:
  using Data = std::string;
  using InterfaceState = ecu::lapi::pb::UdsInterfaceStatus_State;
  using ResponseStatus = ecu::lapi::pb::UdsInterfaceResponse_StatusCode;

  UdsInterface(Client* client, UdsContext&& uds_context);

  UdsInterface(Client* client, const UdsContext& uds_context);

  ~UdsInterface();

  bool GetAllocation(int timeout, bool respond = true) const;

  bool GetRelease(int timeout, bool respond = true) const;

  Data GetRequest(int timeout) const;

  void PublishResponse(Data&& data) const;

  void PublishResponse(const Data& data) const;

  void PublishResponse(const void* data, std::size_t size) const;

  void PublishResponse(ResponseStatus status) const;

  void PublishState(InterfaceState state) const;

 protected:
  std::string GetStatusTopic() const;

  std::string GetResponseTopic() const;

 private:
  Client* client_;
  UdsContext uds_context_;
};

}  // namespace mqtt_util

#endif  // MQTT_UTIL_INCLUDE_MQTT_UTIL_UDSINTERFACE_H_

/************************************************************************************/
