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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_QEMUPROCESS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_QEMUPROCESS_H_

#include "Process.h"

namespace test_util {

class QemuProcess : public Process {
 public:
  QemuProcess(const char* working_dir,
              const char* exec_path,
              const ArgsList& exec_args = {},
              bool quiet = false);
};

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_QEMUPROCESS_H_

/************************************************************************************/
