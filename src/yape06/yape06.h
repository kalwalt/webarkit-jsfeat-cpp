#ifndef YAPE06_H
#define YAPE06_H

#include <cache/Cache.h>
#include <jslog/jslog.h>
#include <keypoint_t/keypoint_t.h>
#include <keypoints/keypoints.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <yape06/yape06_utils.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

namespace jsfeat {

class Yape06 {
public:
  int laplacian_threshold;
  int min_eigen_value_threshold;
  Yape06() {
    laplacian_threshold = 30;
    min_eigen_value_threshold = 25;
  }
  Yape06(int laplacian, int min_eigen) {
    laplacian_threshold = laplacian;
    min_eigen_value_threshold = min_eigen;
  }
  //auto detect_internal(matrix_t *src, Array<keypoint_t> points, int border) {
  auto detect_internal(matrix_t *src, KeyPoints* pts, int border) {
    if (!border) {
      border = 5;
    }
    auto points = pts->kpoints;
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
    /*while (--x >= 0) {
      laplacian[x] = 0;
    }*/
    laplacian = compute_laplacian<u_char, int>(srd_d, w, h, Dxx, Dyy, sx, sy, ex, ey);

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

          min_eigen_value = hessian_min_eigen_value<u_char>(srd_d, rowx, lv,
                                                            Dxx, Dyy, Dxy, Dyx);
          //JSLOGi("min_eigen_value is %d", min_eigen_value);
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
  //auto detect(uintptr_t inputSrc, emscripten::val inputPoints, int border) {
  auto detect(uintptr_t inputSrc, uintptr_t inputPoints, int border) {
    auto src = reinterpret_cast<matrix_t *>(inputSrc);
    //auto points = emscripten::vecFromJSArray<keypoint_t>(inputPoints);
    auto points = reinterpret_cast<KeyPoints *>(inputPoints);

    auto count = detect_internal(src, points, border);
    /*emscripten::val out = emscripten::val::object();
    emscripten::val pointsArr = emscripten::val::array();
    for(auto i = 0; i< points->kpoints.size(); i++){
      pointsArr.call<void>("push", points[i]);
    }
    out.set("count", count);
    out.set("points", pointsArr);

    return out;*/
    return count;
  }
  // getters and setters
  auto getLaplacianThreshold() const { return laplacian_threshold; };
  auto getMinEigenValueThreshold() const { return min_eigen_value_threshold; }
  auto setLaplacianThreshold(int laplacian_value) {
    laplacian_threshold = laplacian_value;
  }
  auto setMinEigenValueThreshold(int threshold_value) {
    min_eigen_value_threshold = threshold_value;
  }
  ~Yape06() {
#ifdef __EMSCRIPTEN__
    JSLOGd("deleting Yape06");
#endif
  }
};

} // namespace jsfeat

#endif