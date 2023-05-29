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

#ifndef TEST_UTIL_INCLUDE_TEST_UTIL_PROCESS_H_
#define TEST_UTIL_INCLUDE_TEST_UTIL_PROCESS_H_

#include <list>

#include "Process.h"

namespace test_util {

class Process {
 public:
  using ArgsList = std::list<const char*>;

  Process(const char* working_dir,
          const char* exec_path,
          const ArgsList& exec_args = {},
          bool quiet = false);

  ~Process();

  int GetPid() const;

  bool SendSignal(int sig_num) const;

  bool Check() const;

  explicit operator bool() const;

 private:
  int pid_;
};

}  // namespace test_util

#endif  // TEST_UTIL_INCLUDE_TEST_UTIL_PROCESS_H_

/************************************************************************************/
