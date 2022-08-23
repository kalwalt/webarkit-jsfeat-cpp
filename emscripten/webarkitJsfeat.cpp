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

void Grayscale_s(u_char *src, int w, int h, Mat_t dst, int code) {
  grayscale_s(src, w, h, dst, code);
}

Mat_t Grayscale_t(int w, int h, int code) {
  u_char some[] = { 23, 20, 12, 24, 212, 220, 120, 46, 78, 92, 35, 12, 120, 120, 120, 120 };
  return grayscale_t(some, w, h, code);
}

_Mat_t Grayscale_tt(int w, int h, int code) {
  u_char some[] = { 23, 20, 12, 24, 212, 220, 120, 46, 78, 92, 35, 12, 120, 120, 120, 120 };
  return grayscale_tt(some, w, h, code);
}

_Mat_t Grayscale_ttm(_Mat_t src, int w, int h, int code) {
  return grayscale_ttm(src, w, h, code);
}

}

#include "bindings.cpp"