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

#ifndef MOCKS_INCLUDE_MOCKS_MOCKBACKENDCLIENT_H_
#define MOCKS_INCLUDE_MOCKS_MOCKBACKENDCLIENT_H_

#include <gmock/gmock.h>
#include <ecu/com/backend.h>

namespace ecu {

namespace lapi {

namespace com {

class MockBackendClient : public BackendClient {
 public:
  explicit MockBackendClient(ITransportClient_ptr transport_client)
      : BackendClient(transport_client) {}

  MOCK_METHOD0(is_connected,
      bool());
  MOCK_METHOD1(add_connection_callback,
      void(ConnectionStateCallback_t));
  MOCK_METHOD2(add_content_callback,
      void(const uint32_t,
           ContentCallback_t));
  MOCK_METHOD1(clear_content_callbacks,
      void(const uint32_t));
  MOCK_METHOD3(publish,
      uint64_t(const Message&,
               const uint32_t,
               const TxOptions&));
};

}  // namespace com

}  // namespace lapi

}  // namespace ecu

#endif  // MOCKS_INCLUDE_MOCKS_MOCKBACKENDCLIENT_H_

/************************************************************************************/
