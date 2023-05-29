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

#include "test_util/QemuProcess.h"

#include <cstdlib>

namespace {

const auto qemu_bin = std::getenv("QEMU_BIN");

using ArgsList = test_util::QemuProcess::ArgsList;

ArgsList PrependArgsList(ArgsList::value_type element, const ArgsList& args_list) {
  auto result = ArgsList{element};
  result.insert(result.end(), args_list.begin(), args_list.end());
  return result;
}

}  // namespace

namespace test_util {

QemuProcess::QemuProcess(const char* working_dir,
                         const char* exec_path,
                         const ArgsList& exec_args,
                         bool quiet)
    : Process(working_dir, qemu_bin, PrependArgsList(exec_path, exec_args), quiet) {}

}  // namespace test_util

/************************************************************************************/
