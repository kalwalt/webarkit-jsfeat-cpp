#include "src/matrix_t/matrix_t.h"
#include <cache/Cache.h>
#include <functional>
#include <imgproc/imgproc.h>
#include <math/math.h>
#include <matrix_t/matrix_t.h>
#include <stdlib.h>
#include <types/types.h>
#include <yape06/yape06.h>
#include <variant>
#include <memory>

using namespace jsfeat;

bool myfunction(int i, int j) { return (i < j); }

int main() {
  Matrix_t *src = new Matrix_t(2, 2, 0x0100 | 0x04);
  Matrix_t *dst = new Matrix_t(2, 2, 0x0100 | 0x01);
  src->allocate();
  u_char some[] = {23, 20, 12, 24, 212, 220, 120, 46,
                   78, 92, 35, 12, 120, 120, 120, 120};
  dst->allocate();
  Imgproc img;
  img.grayscale_m((uintptr_t)src, 2, 2, (uintptr_t)dst,
                  Colors::COLOR_RGBA2GRAY);
  img.grayscale(some, 2, 2, (uintptr_t)dst, Colors::COLOR_RGBA2GRAY);
  Matrix_t test(*src);
  std::cout << "rows " << test.rows << std::endl;
  std::cout << "cols " << test.cols << std::endl;
  std::cout << "type " << test.type << std::endl;
  std::cout << "channel " << test.channel << std::endl;
  std::cout << "first el: " << (int)test.u8[0] << std::endl;
  Cache<3> c;
  std::optional<Data_t> d = c.put_buffer(20, 0x0100 | 0x01);
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
  std::vector<int> res =
      mat1.qsort_internal<int>(vec, 0, vec.size() - 1, myfunction);
  // It should print sorted numbers: 4, 7, 9, 17, 21.
  std::cout << "Sorted numbers with qsort_internal():" << std::endl;
  for (int &v : res)
    std::cout << v << " ";
  std::cout << std::endl;
  std::vector<int> res2 =
      mat1.sort_internal<int, std::greater<int>>(vec, 0, vec.size());
  // It should print sorted numbers: 21, 17, 9, 7, 4.
  std::cout << "Sorted numbers with sort_internal():" << std::endl;
  for (std::vector<int>::iterator it = res2.begin(); it != res2.end(); ++it)
    std::cout << *it << " ";
  std::cout << '\n';

  std::cout << "Yape06 test..." << std::endl;
  Yape06 yape06;
  Matrix_t *yp = new Matrix_t(20, 20, 0x0100 | 0x04);
  yp->allocate();
  KeyPoints *k = new KeyPoints(20);
  KeyPointsCount kp = yape06.detect_internal(yp, k, 5);
  std::cout << "points no.: " << kp.count << std::endl;
  Array<KeyPoint_t> points;
  KeyPoint_t kt(0, 0, 0, 0, 0.0);
  points.assign(5, kt);
  std::cout << points[0].x << std::endl;
  Array<u_char> data { 0, 1, 2, 3 };
  Matrix_t matD(4, 4, 0x0100 | 0x04,  data);
  std::cout << "number at index 2 is: " << (int)matD.u8[2] << std::endl;
  Matrix_t *src_d = new Matrix_t(20, 20, 0x0100 | 0x04); 
  Matrix_t *dst_d = new Matrix_t(20, 20, 0x0100 | 0x04); 
  img.gaussian_blur_internal(src, dst, 5, 2);

  Array<u_char> m_shared_src {23, 45, 14, 45, 126, 89, 245, 78, 178, 12, 224, 95, 120, 205, 120, 2};
  std::shared_ptr<Matrix_t> m_shared_dst = std::make_shared<Matrix_t>(2, 2, 0x0100 | 0x04);
  img.grayscale_rgba_internal(m_shared_src, 2, 2, m_shared_dst);
  std::cout << "Data from grayscale_rgba function" << std::endl;
  for(auto i=0; i<4; i++){
    std::cout << (int)m_shared_dst->u8[i] << std::endl;
  }
}