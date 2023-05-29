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

#include "mqtt_util/UdsInterface.h"

#include <ecu/diag/uds.h>

#include <utility>

#define PROTOBUF_FIELD_EQUAL(obj, field, value) \
  (obj.has_##field() && (obj.field() == value))

namespace mqtt_util {

using UdsClient = ecu::lapi::diag::UdsClient;

UdsInterface::UdsInterface(Client* client, UdsContext&& uds_context)
    : client_(client),
      uds_context_(std::move(uds_context)) {
  assert(client_);
  assert(!uds_context_.client_name.empty());
#if SDK_VERSION_CODE >= GET_SDK_VERSION(7, 7, 0)
  assert(UdsInterface_TransportProtocol_IsValid(uds_context_.transport_protocol));
  assert(UdsInterface_PhysicalInterface_IsValid(uds_context_.physical_interface));
#endif
  assert(uds_context_.source_address);
  assert(uds_context_.target_address);

  client_->Subscribe(UdsClient::TOPIC_UDS_ALLOC);
  client_->Subscribe(UdsClient::TOPIC_UDS_RELEASE);
  client_->Subscribe(UdsClient::TOPIC_UDS_REQ);
}

UdsInterface::UdsInterface(Client* client, const UdsContext& uds_context)
    : UdsInterface(client, UdsContext(uds_context)) {}

UdsInterface::~UdsInterface() {
  client_->Unsubscribe(UdsClient::TOPIC_UDS_ALLOC);
  client_->Unsubscribe(UdsClient::TOPIC_UDS_RELEASE);
  client_->Unsubscribe(UdsClient::TOPIC_UDS_REQ);
}

bool UdsInterface::GetAllocation(int timeout, bool respond) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = client_->GetMessage<ecu::lapi::pb::UdsInterfaceAllocation>(
      UdsClient::TOPIC_UDS_ALLOC, timeout);

  if (!message ||
      !PROTOBUF_FIELD_EQUAL((*message), client_name, uds_context_.client_name)) {
    return false;
  }
#else
  auto message = client_->GetMessage<ecu::lapi::pb::UdsIfMessage>(
      UdsClient::TOPIC_UDS_ALLOC, timeout);

  if (!message || !message->has_interface() || !message->has_allocation()) {
    return false;
  }

  if (!PROTOBUF_FIELD_EQUAL(message->interface(), protocol, uds_context_.transport_protocol) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), phyif, uds_context_.physical_interface) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), target, uds_context_.target_address) ||
      !PROTOBUF_FIELD_EQUAL(message->allocation(), client_name, uds_context_.client_name)) {
    return false;
  }
#endif

  if (respond) {
    PublishState(InterfaceState::UdsInterfaceStatus_State_READY);
  }

  return true;
}

bool UdsInterface::GetRelease(int timeout, bool respond) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = client_->GetMessage<ecu::lapi::pb::UdsInterfaceAllocation>(
      UdsClient::TOPIC_UDS_RELEASE, timeout);

  if (!message ||
      !PROTOBUF_FIELD_EQUAL((*message), client_name, uds_context_.client_name)) {
    return false;
  }
#else
  auto message = client_->GetMessage<ecu::lapi::pb::UdsIfMessage>(
      UdsClient::TOPIC_UDS_RELEASE, timeout);

  if (!message || !message->has_interface() || !message->has_allocation()) {
    return false;
  }

  if (!PROTOBUF_FIELD_EQUAL(message->interface(), protocol, uds_context_.transport_protocol) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), phyif, uds_context_.physical_interface) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), target, uds_context_.target_address) ||
      !PROTOBUF_FIELD_EQUAL(message->allocation(), client_name, uds_context_.client_name)) {
    return false;
  }
#endif

  if (respond) {
    PublishState(InterfaceState::UdsInterfaceStatus_State_FREE);
  }

  return true;
}

UdsInterface::Data UdsInterface::GetRequest(int timeout) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = client_->GetMessage<ecu::lapi::pb::UdsInterfaceRequest>(
      UdsClient::TOPIC_UDS_REQ, timeout);

  if (!message ||
      !PROTOBUF_FIELD_EQUAL((*message), client_name, uds_context_.client_name) ||
      !PROTOBUF_FIELD_EQUAL((*message), target, uds_context_.target_address)) {
    return {};
  }

  auto& data = message->data();
#else
  auto message = client_->GetMessage<ecu::lapi::pb::UdsIfMessage>(
      UdsClient::TOPIC_UDS_REQ, timeout);

  if (!message || !message->has_interface() || !message->has_request()) {
    return {};
  }

  if (!PROTOBUF_FIELD_EQUAL(message->interface(), protocol, uds_context_.transport_protocol) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), phyif, uds_context_.physical_interface) ||
      !PROTOBUF_FIELD_EQUAL(message->interface(), target, uds_context_.target_address) ||
      !PROTOBUF_FIELD_EQUAL(message->request(), client_name, uds_context_.client_name) ||
      !PROTOBUF_FIELD_EQUAL(message->request(), source, uds_context_.source_address)) {
    return {};
  }

  auto& data = message->request().data();
#endif

  return Data(data.begin(), data.end());
}

void UdsInterface::PublishResponse(Data&& data) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = ecu::lapi::pb::UdsInterfaceResponse();
  message.set_source(uds_context_.target_address);
  message.mutable_data()->assign(std::move(data));
#else
  auto message = ecu::lapi::pb::UdsIfMessage();
  auto interface = message.mutable_interface();
  interface->set_protocol(uds_context_.transport_protocol);
  interface->set_phyif(uds_context_.physical_interface);
  interface->set_target(uds_context_.target_address);
  auto response = message.mutable_response();
  response->set_status(ResponseStatus::UdsInterfaceResponse_StatusCode_OK);
  response->set_source(uds_context_.target_address);
  response->mutable_data()->assign(std::move(data));
#endif

  auto topic = GetResponseTopic();
  client_->PublishMessage(topic.c_str(), message);
}

void UdsInterface::PublishResponse(const Data& data) const {
  PublishResponse(Data(data));
}

void UdsInterface::PublishResponse(const void* data, std::size_t size) const {
  PublishResponse(Data(reinterpret_cast<const char*>(data), size));
}

void UdsInterface::PublishResponse(ResponseStatus status) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = ecu::lapi::pb::UdsInterfaceResponse();
  message.set_status(status);
#else
  auto message = ecu::lapi::pb::UdsIfMessage();
  auto interface = message.mutable_interface();
  interface->set_protocol(uds_context_.transport_protocol);
  interface->set_phyif(uds_context_.physical_interface);
  interface->set_target(uds_context_.target_address);
  auto response = message.mutable_response();
  response->set_status(status);
#endif

  auto topic = GetResponseTopic();
  client_->PublishMessage(topic.c_str(), message);
}

void UdsInterface::PublishState(InterfaceState state) const {
#if SDK_VERSION_CODE < GET_SDK_VERSION(7, 7, 0)
  auto message = ecu::lapi::pb::UdsInterfaceStatus();
  message.set_state(state);
#else
  auto message = ecu::lapi::pb::UdsIfMessage();
  auto interface = message.mutable_interface();
  interface->set_protocol(uds_context_.transport_protocol);
  interface->set_phyif(uds_context_.physical_interface);
  interface->set_target(uds_context_.target_address);
  auto status = message.mutable_status();
  status->set_state(state);
#endif

  auto topic = GetStatusTopic();
  client_->PublishMessage(topic.c_str(), message);
}

std::string UdsInterface::GetStatusTopic() const {
  return "app/" + uds_context_.client_name + "/uds/status";
}

std::string UdsInterface::GetResponseTopic() const {
  return "app/" + uds_context_.client_name + "/uds/response";
}

}  // namespace mqtt_util

/************************************************************************************/
