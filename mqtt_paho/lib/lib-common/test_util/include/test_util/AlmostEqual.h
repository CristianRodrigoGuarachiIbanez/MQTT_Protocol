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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_ALMOSTEQUAL_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_ALMOSTEQUAL_H_

namespace test_util {

template<class T>
bool AlmostEqual(const T& a, const T& b, const T& max_diff) {
  return ((a > b) ? (a - b) : (b - a)) < max_diff;
}

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_ALMOSTEQUAL_H_

/************************************************************************************/
