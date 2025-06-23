// mct-dl.h
// single-file C wrapper for loading dynamic libraries (.so/.dll/.dylib) and
// feching symbols from said library.
// Eduardo Costa
//
// LICENSE - see end of file
//
// to create implementation, add
// #define MCT_DL_IMPLEMENTATION
// in one C/CPP file that includes this file
//
// Example:
//
// #define MCT_DL_IMPLEMENTATION
// #include "mct-dl.h"
//
// void example() {
//   void * lib = mct_dlopen("mylib"); // no extension
//   void (*hello)() = mct_dlsym(lib, "hello");
//   hello();
//   mct_dlclose(lib);
// }

#ifndef MCT_DL_H
#define MCT_DL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * mct_dl_t;
mct_dl_t * mct_dlopen(const char * name);
void * mct_dlsym(mct_dl_t * lib, const char * symname);
void mct_dlclose(mct_dl_t * lib);

#ifdef MCT_DL_IMPLEMENTATION

#if WIN32
#include <windows.h>

mct_dl_t * mct_dlopen(const char * name) { return LoadLibraryA(name); }
void * mct_dlsym(mct_dl_t * lib, const char * symname) { return GetProcAddress((HANDLE) lib, symname); }
void mct_dlclose(mct_dl_t * lib) { FreeLibrary((HANDLE) lib); }

#else // not WIN32
#include <dlfcn.h>
#include <string.h>

mct_dl_t * mct_dlopen(const char * name) {
  char buf[1024];
  strncpy(buf, name, 1024);
#ifdef __APPLE__
  strncat(buf, ".dylib", 1024);
#else
  strncat(buf, ".so", 1024);
#endif
  return dlopen(buf, RTLD_NOW | RTLD_LOCAL);
}
void * mct_dlsym(mct_dl_t * lib, const char * symname) { return dlsym(lib, symname); }
void mct_dlclose(mct_dl_t * lib) { dlclose(lib); }

#endif // WIN32
#endif // MCT_DL_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // MCT_DL_H

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
