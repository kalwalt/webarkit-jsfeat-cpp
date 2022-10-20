#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <imgproc/imgproc.h>
#include <stdlib.h>
#include <cache/Cache.h>

using namespace jsfeat;

int main() {
  matrix_t *src = new matrix_t(2, 2, 0x0100 | 0x04);
  matrix_t *dst = new matrix_t(2, 2, 0x0100 | 0x01);
  src->allocate();
  u_char some[] = { 23, 20, 12, 24, 212, 220, 120, 46, 78, 92, 35, 12, 120, 120, 120, 120 };
  //src->data = some;
  //src->fill(some);
  dst->allocate();
  imgproc img;
  img.grayscale_m((uintptr_t)src, 2, 2, (uintptr_t)dst, Colors::COLOR_RGBA2GRAY);
  img.grayscale(some, 2, 2, (uintptr_t)dst, Colors::COLOR_RGBA2GRAY);
  matrix_t test(*src);
  std::cout << "rows " << test.rows << std::endl;
  std::cout << "cols " << test.cols << std::endl;
  std::cout << "type " << test.type << std::endl;
  std::cout << "channel " << test.channel << std::endl;
  std::cout << "first el: " << (int)test.u8[0] << std::endl;
  Cache<3> c;
  c.put_buffer(20, 0x0100 | 0x01);
  std::optional<data_t> d = c.get("buffer_0");
  d->u8[0] = 120;
  std::cout << (int)d->u8[0] << std::endl;
}