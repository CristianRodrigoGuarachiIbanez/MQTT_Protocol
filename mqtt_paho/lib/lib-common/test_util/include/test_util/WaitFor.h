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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_WAITFOR_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_WAITFOR_H_

#include <functional>

namespace test_util {

bool WaitFor(int timeout, const std::function<bool(int)>& subroutine);

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_WAITFOR_H_

/************************************************************************************/
