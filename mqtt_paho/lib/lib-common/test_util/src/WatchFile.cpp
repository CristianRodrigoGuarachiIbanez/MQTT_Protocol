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

#include "test_util/WatchFile.h"

#include <cassert>

#include <chrono>  // NOLINT
#include <string>
#include <utility>

#include "test_util/InotifyWrapper.h"
#include "test_util/WaitFor.h"

namespace {

std::pair<std::string, std::string> SplitPath(const char* path) {
  assert(path);

  auto tmp = std::string(path);

  auto slash = tmp.rfind('/');
  if (slash == tmp.npos) {
    return std::make_pair(".", tmp);
  }

  return std::make_pair(tmp.substr(0, slash), tmp.substr(slash + 1));
}

}  // namespace

namespace test_util {

bool WatchFile(const char* path, int timeout,
               const std::function<void()>& subroutine) {
  auto split = SplitPath(path);

  auto inotify = InotifyWrapper(split.first.c_str(),
      IN_CREATE | IN_MODIFY | IN_MOVE | IN_DELETE | IN_CLOSE_WRITE | IN_ONLYDIR);

  if (subroutine) {
    subroutine();
  }

  auto combined_mask = std::uint32_t();

  return WaitFor(timeout, [&](int remaining) {
    auto data = inotify.ReadData(remaining);
    if (data.empty()) {
      return false;
    }

    auto events = inotify.GetEventList(data);
    for (auto event : events) {
      assert(event);

      if (event->len && (event->name == split.second)) {
        combined_mask |= event->mask;
      }
    }

    if (combined_mask & (IN_MOVE | IN_DELETE | IN_CLOSE_WRITE)) {
      if (combined_mask & (IN_CREATE | IN_MODIFY | IN_MOVE | IN_DELETE)) {
        return true;
      }

      combined_mask = 0;
    }

    return false;
  });
}

}  // namespace test_util

/************************************************************************************/
