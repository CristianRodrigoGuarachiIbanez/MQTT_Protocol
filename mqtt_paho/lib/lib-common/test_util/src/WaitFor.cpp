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

#include "test_util/WaitFor.h"

#include <cassert>

#include <chrono>  // NOLINT

namespace test_util {

bool WaitFor(int timeout, const std::function<bool(int)>& subroutine) {
  assert(subroutine);

  auto remaining = std::chrono::milliseconds(timeout);
  auto deadline = std::chrono::steady_clock::now() + remaining;

  while (remaining >= remaining.zero()) {
    if (subroutine(remaining.count())) {
      return true;
    }

    remaining = std::chrono::duration_cast<decltype(remaining)>(
        deadline - decltype(deadline)::clock::now());
  }

  return false;
}

}  // namespace test_util

/************************************************************************************/
