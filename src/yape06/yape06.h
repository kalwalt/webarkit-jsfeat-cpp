#ifndef YAPE06_H
#define YAPE06_H

#include <cache/Cache.h>
#include <keypoint_t/keypoint_t.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <yape06/yape06_utils.h>

namespace jsfeat {

class Yape06 {
public:
  int laplacian_threshold;
  int min_eigen_value_threshold;
  Yape06() {
    laplacian_threshold = 30;
    min_eigen_value_threshold = 25;
  }
  auto detect(matrix_t *src, Array<keypoint_t> points, int border) {
    if (!border) {
      border = 5;
    }
    auto x = 0, y = 0;
    auto w = src->cols, h = src->rows;
    // auto srd_d = src.data;
    auto srd_d = src->u8;
    auto Dxx = 5, Dyy = (5 * w) | 0;
    auto Dxy = (3 + 3 * w) | 0, Dyx = (3 - 3 * w) | 0;
    auto lap_buf = cache.put_buffer((w * h) << 2, Types::S32_t | Types::C1_t);
    auto laplacian = lap_buf->i32;
    auto lv = 0, row = 0, rowx = 0, min_eigen_value = 0;
    // Array<keypoint_t> pt;
    auto number_of_points = 0;
    auto lap_thresh = laplacian_threshold;
    auto eigen_thresh = min_eigen_value_threshold;

    auto sx = std::max(5, border) | 0;
    auto sy = std::max(3, border) | 0;
    auto ex = std::min(w - 5, w - border) | 0;
    auto ey = std::min(h - 3, h - border) | 0;

    x = w * h;
    while (--x >= 0) {
      laplacian[x] = 0;
    }
    compute_laplacian<u_char, int>(srd_d, laplacian, w, h, Dxx, Dyy, sx, sy, ex, ey);

    row = (sy * w + sx) | 0;
    for (y = sy; y < ey; ++y, row += w) {
      for (x = sx, rowx = row; x < ex; ++x, ++rowx) {

        lv = laplacian[rowx];
        if ((lv < -lap_thresh && lv < laplacian[rowx - 1] &&
             lv < laplacian[rowx + 1] && lv < laplacian[rowx - w] &&
             lv < laplacian[rowx + w] && lv < laplacian[rowx - w - 1] &&
             lv < laplacian[rowx + w - 1] && lv < laplacian[rowx - w + 1] &&
             lv < laplacian[rowx + w + 1]) ||
            (lv > lap_thresh && lv > laplacian[rowx - 1] &&
             lv > laplacian[rowx + 1] && lv > laplacian[rowx - w] &&
             lv > laplacian[rowx + w] && lv > laplacian[rowx - w - 1] &&
             lv > laplacian[rowx + w - 1] && lv > laplacian[rowx - w + 1] &&
             lv > laplacian[rowx + w + 1])) {

          min_eigen_value =
              hessian_min_eigen_value<u_char>(srd_d, rowx, lv, Dxx, Dyy, Dxy, Dyx);
          if (min_eigen_value > eigen_thresh) {
            auto pt = points[number_of_points];
            pt.x = x, pt.y = y, pt.score = min_eigen_value;
            ++number_of_points;
            ++x, ++rowx; // skip next pixel since this is maxima in 3x3
          }
        }
      }
    }

    // this.cache.put_buffer(lap_buf);

    return number_of_points;
  }
};

} // namespace jsfeat

#endif