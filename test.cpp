#include <cache/Cache.h>
#include <imgproc/imgproc.h>
#include <math/math.h>
#include <matrix_t/matrix_t.h>
#include <stdlib.h>
#include <types/types.h>

using namespace jsfeat;

bool myfunction(int i, int j) { return (i < j); }

int main() {
  matrix_t *src = new matrix_t(2, 2, 0x0100 | 0x04);
  matrix_t *dst = new matrix_t(2, 2, 0x0100 | 0x01);
  src->allocate();
  u_char some[] = {23, 20, 12, 24, 212, 220, 120, 46, 78, 92, 35, 12, 120, 120, 120, 120};
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
  std::optional<data_t> d = c.put_buffer(20, 0x0100 | 0x01);
  d->u8[0] = 120;
  std::cout << (int)d->u8[0] << std::endl;
  c.back_buffer();
  d->u8.clear();
  std::cout << "vector cleared" << std::endl;
  // data are not cleared...
  std::cout << (int)d->u8[0] << std::endl;
  std::cout << "Numbers to be sorted: 17, 9, 4, 7, 21." << std::endl;
  Math mat1;
  std::vector<int> vec{17, 9, 4, 7, 21};
  std::vector<int> res = mat1.qsort_internal<int>(vec, 0, vec.size() - 1, myfunction);
  // It should print sorted numbers: 4, 7, 9, 17, 21,
  std::cout << "Sorted numbers:" << std::endl;
  for (int &v : res)
    std::cout << v << " ";
    std::cout << std::endl;
  std::vector<int> res2 = mat1.sort_internal<int>(vec, 0, vec.size() - 1);
  // It should print sorted numbers: 4, 7, 9, 17, 21,
  std::cout << "Sorted numbers:" << std::endl;
  for (std::vector<int>::iterator it=res2.begin(); it!=res2.end(); ++it)
    std::cout << *it << " ";
    std::cout << '\n';
}