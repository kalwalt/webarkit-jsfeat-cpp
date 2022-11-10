#ifndef TYPES_H
#define TYPES_H

#include <cstdio>
#include <vector>
#include <variant>
#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif
namespace jsfeat {

using u_char = unsigned char;
using u_int = unsigned int;
template <typename T> using Array = std::vector<T>;
using Data =
      std::variant<Array<u_char>, Array<int>, Array<float>, Array<double>>;
#ifdef __EMSCRIPTEN__
thread_local const emscripten::val Uint8Array =
    emscripten::val::global("Uint8Array");
thread_local const emscripten::val Float32Array =
    emscripten::val::global("Float32Array");
struct _Mat_t {
  int size;
  int cols;
  int rows;
  int channels;
  emscripten::val data = Uint8Array;
};
#endif

struct KPoint_t;
using ItemKPoint_t = Array<KPoint_t>;
struct KPoint_t {
  int x;
  int y;
  int level;
  int score;
  float angle;
};

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