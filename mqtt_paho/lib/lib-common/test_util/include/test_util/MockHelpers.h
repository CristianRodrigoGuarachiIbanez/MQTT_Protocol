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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_MOCKHELPERS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_MOCKHELPERS_H_

#include <gmock/gmock.h>

#include <memory>
#include <utility>

namespace test_util {

template<class T, class... Args>
std::unique_ptr<T> MakeMock(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T, class... Args>
std::unique_ptr<T> MakeNiceMock(Args&&... args) {
  return std::unique_ptr<T>(new ::testing::NiceMock<T>(std::forward<Args>(args)...));
}

template<class T, class... Args>
std::unique_ptr<T> MakeStrictMock(Args&&... args) {
  return std::unique_ptr<T>(new ::testing::StrictMock<T>(std::forward<Args>(args)...));
}

template<class T>
std::shared_ptr<T> ToShared(std::unique_ptr<T>&& ptr) {
  return std::shared_ptr<T>(std::move(ptr));
}

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_MOCKHELPERS_H_

/************************************************************************************/
