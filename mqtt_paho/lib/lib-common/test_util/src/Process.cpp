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

#include "test_util/Process.h"

#include <sys/prctl.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include <cassert>

#include <vector>

#include "test_util/GenerateError.h"

namespace test_util {

Process::Process(const char* working_dir,
                 const char* exec_path,
                 const ArgsList& exec_args,
                 bool quiet)
    : pid_(-1) {
  assert(exec_path);

  auto pid = fork();
  switch (pid) {
    case -1:
      throw GenerateError("Unable to fork", errno);

    case 0: {
      if (working_dir && ::chdir(working_dir) != 0) {
        std::exit(EXIT_FAILURE);
      }

      if (::prctl(PR_SET_PDEATHSIG, SIGKILL) != 0) {
        std::exit(EXIT_FAILURE);
      }

      if (quiet) {
        auto fd = ::open("/dev/null", O_RDWR);
        if (fd < 0) {
          std::exit(EXIT_FAILURE);
        }

        if (::dup2(fd, STDIN_FILENO) < 0) {
          std::exit(EXIT_FAILURE);
        }

        if (::dup2(fd, STDOUT_FILENO) < 0) {
          std::exit(EXIT_FAILURE);
        }

        if (::dup2(fd, STDERR_FILENO) < 0) {
          std::exit(EXIT_FAILURE);
        }

        if (::close(fd) != 0) {
          std::exit(EXIT_FAILURE);
        }
      }

      auto args = std::vector<const char*>{exec_path};
      args.insert(args.end(), exec_args.begin(), exec_args.end());
      args.push_back(nullptr);

      ::execvp(args[0], const_cast<char* const*>(args.data()));

      std::exit(EXIT_FAILURE);
    }

    default:
      pid_ = pid;
  }
}

Process::~Process() {
  if (Check()) {
    ::kill(pid_, SIGTERM);
    ::waitpid(pid_, nullptr, 0);
  }
}

int Process::GetPid() const {
  return pid_;
}

bool Process::SendSignal(int sig_num) const {
  return (::kill(pid_, sig_num) == 0);
}

bool Process::Check() const {
  return (::waitpid(pid_, nullptr, WNOHANG) == 0);
}

Process::operator bool() const {
  return Check();
}

}  // namespace test_util

/************************************************************************************/
