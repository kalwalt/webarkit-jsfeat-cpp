#ifndef TYPES_H
#define TYPES_H

#include <cstdio>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif
namespace jsfeat {
typedef unsigned char u_char;
template <typename T> using Array = std::vector<T>;
#ifdef __EMSCRIPTEN__
thread_local const emscripten::val Uint8Array = emscripten::val::global("Uint8Array");

struct _Mat_t {
  int size;
  int cols;
  int rows;
  int channels;
  emscripten::val data = Uint8Array;
};
#endif

enum Types {
  U8_t = 0x0100,
  S32_t = 0x0200,
  F32_t = 0x0400,
  S64_t = 0x0800,
  F64_t = 0x1000,
  C1_t = 0x01,
  C2_t = 0x02,
  C3_t = 0x03,
  C4_t = 0x04
};

enum Colors {
  COLOR_RGBA2GRAY = 0,
  COLOR_RGB2GRAY = 1,
  COLOR_BGRA2GRAY = 2,
  COLOR_BGR2GRAY = 3
};
} // namespace jsfeat

#endif