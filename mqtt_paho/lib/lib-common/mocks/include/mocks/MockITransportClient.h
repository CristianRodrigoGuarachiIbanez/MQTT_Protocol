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

#ifndef MOCKS_INCLUDE_MOCKS_MOCKITRANSPORTCLIENT_H_
#define MOCKS_INCLUDE_MOCKS_MOCKITRANSPORTCLIENT_H_

#include <gmock/gmock.h>
#include <ecu/com/client.h>

#include <string>
#include <vector>

namespace ecu {

namespace lapi {

namespace com {

class MockITransportClient : public ITransportClient {
 public:
  MOCK_METHOD0(connect,
      bool());
  MOCK_METHOD0(disconnect,
      void());
  MOCK_METHOD0(isConnected,
      bool());
  MOCK_METHOD2(publish,
      int(const std::string&,
          const Message&));
  MOCK_METHOD3(publish_wait_ack,
      int(const std::string&,
          const Message&,
          const std::chrono::seconds));
  MOCK_METHOD3(subscribe,
      void(const std::string&,
           uint8_t,
           ISubscriptionObserver_ptr));
  MOCK_METHOD2(unsubscribe,
      void(const std::string&,
           ISubscriptionObserver_ptr));
  MOCK_METHOD4(subscribe_and_announce,
      void(const std::vector<std::string>&,
           uint8_t,
           ISubscriptionObserver_ptr,
           Message&));
  MOCK_METHOD1(add_connection_observer,
      void(IConnectionObserver_ptr));
  MOCK_METHOD1(run,
      void(bool));
  MOCK_METHOD0(start,
      void());
  MOCK_METHOD0(stop,
      void());
  MOCK_METHOD0(join,
      void());
  MOCK_CONST_METHOD0(client_name,
      const std::string&());
  MOCK_METHOD2(set_will,
      bool(const std::string&,
           const Message&));
  MOCK_METHOD0(clear_will,
      bool());
  MOCK_METHOD0(is_ready,
      bool());
  MOCK_METHOD1(wait_ready,
      bool(const std::chrono::seconds));
};

}  // namespace com

}  // namespace lapi

}  // namespace ecu

#endif  // MOCKS_INCLUDE_MOCKS_MOCKITRANSPORTCLIENT_H_

/************************************************************************************/
