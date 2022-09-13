#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <imgproc/imgproc.h>
#include <stdlib.h>

using namespace jsfeat;

int main() {
  matrix_t *src = new matrix_t(2, 2, 0x0100 | 0x04, 0x0100);
  matrix_t *dst = new matrix_t(2, 2, 0x0100 | 0x01, 0x0100);
  src->allocate();
  u_char some[] = { 23, 20, 12, 24, 212, 220, 120, 46, 78, 92, 35, 12, 120, 120, 120, 120 };
  //src->data = some;
  //src->fill(some);
  dst->allocate();
  imgproc img;
  img.grayscale_m((uintptr_t)src, 2, 2, (uintptr_t)dst, Colors::COLOR_RGBA2GRAY);
  img.grayscale(some, 2, 2, (uintptr_t)dst, Colors::COLOR_RGBA2GRAY);
}