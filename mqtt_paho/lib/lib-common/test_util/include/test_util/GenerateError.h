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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_GENERATEERROR_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_GENERATEERROR_H_

#include <stdexcept>

namespace test_util {

std::runtime_error GenerateError(const char* cause, int errnum = 0);

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_GENERATEERROR_H_

/************************************************************************************/
