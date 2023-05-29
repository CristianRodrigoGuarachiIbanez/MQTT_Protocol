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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_TIMESTAMPHELPERS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_TIMESTAMPHELPERS_H_

#include <chrono>  // NOLINT

#include "AlmostEqual.h"

namespace test_util {

template<class Clock = std::chrono::system_clock>
std::uint64_t GetTimestamp() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      Clock::now().time_since_epoch()).count();
}

template<class Clock = std::chrono::system_clock>
bool VerifyTimestamp(std::uint64_t timestamp, std::uint32_t max_diff = 200) {
  auto now = GetTimestamp<Clock>();

  return AlmostEqual<decltype(now)>(now, timestamp, max_diff);
}

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_TIMESTAMPHELPERS_H_

/************************************************************************************/
