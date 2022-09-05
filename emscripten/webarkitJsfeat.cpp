#include <emscripten.h>
#include <jsfeat.h>

using namespace jsfeat;

extern "C" {

void Grayscale(u_char *src, int w, int h, matrix_t *dst, int code) {
  grayscale(src, w, h, dst, code);
}

void Grayscale_m(matrix_t *src, int w, int h, matrix_t *dst, int code) {
  grayscale_m(src, w, h, dst, code);
}
}

#include "bindings.cpp"