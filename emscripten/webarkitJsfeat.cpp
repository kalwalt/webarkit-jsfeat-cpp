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

std::vector<u_char> Grayscale_t(u_char *src, int w, int h, int code) {
  Mat_t m;
  m = grayscale_t(src, w, h, code);
  return m.data; 
}

}

#include "bindings.cpp"