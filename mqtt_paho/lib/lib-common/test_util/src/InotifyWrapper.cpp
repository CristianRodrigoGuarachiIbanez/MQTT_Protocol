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

#include "test_util/InotifyWrapper.h"

#include <linux/limits.h>
#include <poll.h>
#include <unistd.h>

#include <cassert>

#include "test_util/GenerateError.h"

namespace test_util {

InotifyWrapper::InotifyWrapper(const char* path, std::uint32_t mask) {
  assert(path);
  assert(mask);

  inotify_fd_ = ::inotify_init();
  if (inotify_fd_ < 0) {
    throw GenerateError("Unable to initialize inotify descriptor", errno);
  }

  file_wd_ = ::inotify_add_watch(inotify_fd_, path, mask);
  if (file_wd_ < 0) {
    throw GenerateError("Unable to add inotify watch", errno);
  }
}

InotifyWrapper::~InotifyWrapper() {
  ::close(inotify_fd_);
}

InotifyWrapper::InotifyData InotifyWrapper::ReadData(int timeout) const {
  auto poll_fds = ::pollfd{inotify_fd_, POLLIN, 0};
  auto poll_result = ::poll(&poll_fds, 1, timeout);
  switch (poll_result) {
    case -1:
      throw GenerateError("Unable to poll inotify descriptor", errno);

    case 0:
      return {};

    default:
      break;
  }

  auto read_data = InotifyData(sizeof(::inotify_event) + NAME_MAX + 1);
  auto read_result = ::read(inotify_fd_, read_data.data(), read_data.size());
  if (read_result < 0) {
    throw GenerateError("Unable to read inotify descriptor", errno);
  }

  read_data.resize(read_result);

  return read_data;
}

InotifyWrapper::InotifyEventList InotifyWrapper::GetEventList(
    const InotifyData& data) {
  return GetEventList(data.data(), data.size());
}

InotifyWrapper::InotifyEventList InotifyWrapper::GetEventList(
    const void* data, std::size_t size) {
  auto result = InotifyEventList();

  auto offset = decltype(size)();
  while (offset < size) {
    auto ptr = InotifyEventList::value_type(static_cast<const char*>(data) + offset);

    result.push_back(ptr);

    offset += sizeof(*ptr) + ptr->len;
  }

  assert(offset == size);

  return result;
}

}  // namespace test_util

/************************************************************************************/
