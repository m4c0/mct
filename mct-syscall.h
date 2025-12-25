// mct-syscall.h
// single-file C wrapper for a unified interface of some basic libc calls by
// Eduardo Costa
//
// LICENSE - see end of file
//
// to create implementation, add
// #define MCT_SYSCALL_IMPLEMENTATION
// in one C/CPP file that includes this file
//

#ifndef MCT_SYSCALL_H
#define MCT_SYSCALL_H

// TODO: find ways of only including those in implementation
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

int          mct_syscall_chdir(const char * path);
/// Returns a copy of an environment variable or NULL if it does not exist
char *       mct_syscall_dupenv(const char * name);
FILE *       mct_syscall_fopen(const char * name, const char * mode);
void         mct_syscall_fullpath(const char * path, char * dst, unsigned dst_len);
struct tm *  mct_syscall_gmtime(const time_t * t);
/// Creates a hard link to a file
const char * mct_syscall_link(const char * src, const char * dst);
int          mct_syscall_mkdir(const char * path);
void         mct_syscall_setenv(const char * name, const char * value);
int          mct_syscall_spawn(const char * cmd, const char * const * argv);
char *       mct_syscall_strdup(const char * str);

#ifdef MCT_SYSCALL_IMPLEMENTATION

#ifdef _WIN32
#  include <direct.h>
#  include <process.h>
#  include <stdlib.h>
#  include <time.h>

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else // !_WIN32
#  include <errno.h>
#  include <stdlib.h>
#  include <string.h>
#  include <sys/stat.h>
#  include <unistd.h>

#  ifdef __APPLE__
#    include <sys/clonefile.h>
#  endif
#endif

int mct_syscall_chdir(const char * path) {
#ifdef _WIN32
  return _chdir(path);
#else
  return chdir(path);
#endif
}

char * mct_syscall_dupenv(const char * name) {
#ifdef _WIN32
  char *buf;
  size_t sz;
  return (0 == _dupenv_s(&buf, &sz, name)) ? buf : NULL;
#else
  const char * e = getenv(name);
  return e ? strdup(e) : NULL;
#endif
}

FILE * mct_syscall_fopen(const char * name, const char * mode) {
#ifdef _WIN32
  FILE * res;
  return (0 == fopen_s(&res, name, mode)) ? res : NULL;
#else
  return fopen(name, mode);
#endif
}

void mct_syscall_fullpath(const char * path, char * dst, unsigned dst_size) {
#ifdef _WIN32
  _fullpath(dst, path, dst_size);
#else
  // TODO: assert dst_size >= PATH_MAX
  realpath(path, dst);
#endif
}

struct tm * mct_syscall_gmtime(const time_t * t) {
#ifdef _WIN32
  static struct tm tm;
  return (0 == gmtime_s(&tm, t)) ? &tm : NULL;
#else
  return gmtime(t);
#endif
}

const char * mct_syscall_link(const char * src, const char * dst) {
#ifdef _WIN32
  if (CreateHardLink(dst, src, NULL)) return NULL;

  static char buf[1024];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), LOCALE_USER_DEFAULT, buf, sizeof(buf), NULL);
  return buf;
#else
  // TODO: confirm if this works on iOS
  if (link(src, dst) == 0) return NULL;
  return strerror(errno);
#endif
}

int mct_syscall_mkdir(const char * path) {
#ifdef _WIN32
  return _mkdir(path);
#else
  return mkdir(path, 0777);
#endif
}

void mct_syscall_setenv(const char * name, const char * value) {
#ifdef _WIN32
  SetEnvironmentVariable(name, value);
#else
  setenv(name, value, 0);
#endif
}

int mct_syscall_spawn(const char * cmd, const char * const * argv) {
#ifdef _WIN32
  return _spawnvp(_P_WAIT, cmd, argv);
#else
  pid_t pid = fork();
  if (pid < 0) return pid;
  if (pid == 0) {
    execvp(cmd, (char * const *)argv);
    exit(1);
  } else {
    int res;
    waitpid(pid, &res, 0);
    if (WIFEXITED(res)) return WEXITSTATUS(res);
    return 1;
  }
#endif
}

char * mct_syscall_strdup(const char * str) {
#ifdef _WIN32
  return _strdup(str);
#else
  return strdup(str);
#endif
}

#endif // MCT_SYSCALL_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#endif // MCT_SYSCALL_H

// MIT License
//
// Copyright (c) 2025 Eduardo Costa
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
