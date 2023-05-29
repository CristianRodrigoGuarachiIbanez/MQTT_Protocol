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

#include "test_util/InitLogging.h"

#include <ecu/logging.h>

namespace {

const auto coverage = std::getenv("COVERAGE");

const auto log_level = std::getenv("LOGLEVEL");

}  // namespace

namespace test_util {

void InitLogging() {
  ecu::lapi::logging::set_log_mode(ecu::lapi::logging::LogMode::STDOUT);

  if (coverage) {
    ecu::lapi::logging::set_log_level(LOG_DEBUG);
  } else if (log_level) {
    ecu::lapi::logging::set_log_level(log_level);
  } else {
    ecu::lapi::logging::set_log_level(LOG_EMERG);
  }
}

}  // namespace test_util

/************************************************************************************/
