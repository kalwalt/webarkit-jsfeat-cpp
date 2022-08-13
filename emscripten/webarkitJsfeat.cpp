#include <emscripten.h>
#include <jsfeat.h>

using namespace jsfeat;

extern "C" {

int toGrayscale(u_char *src, int w, int h, matrix_t *dst, int code) {
  grayscale(src, w, h, dst, code);
  return 1;
};

void Grayscale(u_char *src, int w, int h, matrix_t *dst, int code) {
  grayscale(src, w, h, dst, code);
}

}

#include "bindings.cpp"