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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_INOTIFYWRAPPER_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_INOTIFYWRAPPER_H_

#include <sys/inotify.h>

#include <cstdint>

#include <list>
#include <vector>

namespace test_util {

class InotifyWrapper {
 public:
  using InotifyData = std::vector<char>;
  using InotifyEventList = std::list<const ::inotify_event*>;

  InotifyWrapper(const char* path, std::uint32_t mask);

  ~InotifyWrapper();

  InotifyData ReadData(int timeout) const;

  static InotifyEventList GetEventList(const InotifyData& data);

  static InotifyEventList GetEventList(const void* data, std::size_t size);

 private:
  int inotify_fd_;
  int file_wd_;
};

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_INOTIFYWRAPPER_H_

/************************************************************************************/
