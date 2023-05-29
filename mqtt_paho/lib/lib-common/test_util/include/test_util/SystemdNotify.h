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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_SYSTEMDNOTIFY_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_SYSTEMDNOTIFY_H_

#include <string>

namespace test_util {

class SystemdNotify {
 public:
  SystemdNotify();

  ~SystemdNotify();

  std::string GetNotification(int timeout) const;

 private:
  int socket_;
};

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_SYSTEMDNOTIFY_H_

/************************************************************************************/
