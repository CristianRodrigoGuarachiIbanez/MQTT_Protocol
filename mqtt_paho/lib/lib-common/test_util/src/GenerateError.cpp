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

#include "test_util/GenerateError.h"

#include <cassert>
#include <cstring>

#include <string>

namespace test_util {

std::runtime_error GenerateError(const char* cause, int errnum) {
  assert(cause);

  auto description = std::string(cause);
  if (errnum) {
    description.append(": ").append(::strerror(errnum));
  }

  return std::runtime_error(description);
}

}  // namespace test_util

/************************************************************************************/
