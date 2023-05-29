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

#include "test_util/SystemdNotify.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>
#include <unistd.h>

#include "test_util/GenerateError.h"

namespace {

constexpr auto sock_addr = ::sockaddr_un{AF_UNIX, "/tmp/systemd_notify_socket"};

}  // namespace

namespace test_util {

SystemdNotify::SystemdNotify() {
  ::unlink(sock_addr.sun_path);

  socket_ = ::socket(AF_UNIX, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (socket_ < 0) {
    throw GenerateError("Unable to open systemd notify socket", errno);
  }

  if (::bind(socket_, reinterpret_cast<const ::sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0) {
    throw GenerateError("Unable to bind systemd notify socket", errno);
  }

  if (::setenv("NOTIFY_SOCKET", sock_addr.sun_path, 0) < 0) {
    throw GenerateError("Unable to set NOTIFY_SOCKET environment variable", errno);
  }
}

SystemdNotify::~SystemdNotify() {
  if (socket_ >= 0) {
    ::close(socket_);
  }

  ::unlink(sock_addr.sun_path);
}

std::string SystemdNotify::GetNotification(int timeout) const {
  auto poll_fds = ::pollfd{socket_, POLLIN, 0};
  auto poll_result = ::poll(&poll_fds, 1, timeout);
  switch (poll_result) {
    case -1:
      throw GenerateError("Unable to poll systemd notify socket", errno);

    case 0:
      return {};

    default:
      break;
  }

  auto result = std::string(64, ' ');
  auto received = ::recv(socket_, &result.front(), result.size(), MSG_DONTWAIT);
  if (received < 0) {
    throw GenerateError("Unable to read systemd notify socket", errno);
  }

  result.resize(received);

  return result;
}

}  // namespace test_util

/************************************************************************************/
