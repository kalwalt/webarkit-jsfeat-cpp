#ifndef YAPE06_H
#define YAPE06_H

#include <cache/Cache.h>
#include <jslog/jslog.h>
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
  auto detect_internal(Matrix_t* src, KeyPoints* pts, int border) {
    if (!border) {
      border = 5;
    }
    assert(border > 0 && "border value must be > o!");
    auto points = pts->kpoints;
    KeyPointsCount ypts(*pts);
    auto x = 0, y = 0;
    auto w = src->cols, h = src->rows;
    // auto srd_d = src.data;
    auto srd_d = src->u8;
    auto Dxx = 5, Dyy = (5 * w) | 0;
    auto Dxy = (3 + 3 * w) | 0, Dyx = (3 - 3 * w) | 0;
    auto lap_buf = cache.put_buffer((w * h) << 2, Types::S32_t | Types::C1_t);
    auto laplacian = lap_buf->i32;
    auto lv = 0, row = 0, rowx = 0, min_eigen_value = 0;
    auto number_of_points = 0;
    auto lap_thresh = laplacian_threshold;
    auto eigen_thresh = min_eigen_value_threshold;

    auto sx = std::max(5, border) | 0;
    auto sy = std::max(3, border) | 0;
    auto ex = std::min(w - 5, w - border) | 0;
    auto ey = std::min(h - 3, h - border) | 0;

    x = w * h;

    compute_laplacian<u_char, int>(srd_d, laplacian, w, h, Dxx, Dyy, sx, sy, ex,
                                   ey);

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

          if (min_eigen_value > eigen_thresh) {
            ypts.count = number_of_points,
            ypts.pts.kpoints[number_of_points].x = x,
            ypts.pts.kpoints[number_of_points].y = y,
            ypts.pts.kpoints[number_of_points].score = min_eigen_value;
            ++number_of_points;
            ++x, ++rowx; // skip next pixel since this is maxima in 3x3
          }
        }
      }
    }

    return ypts;
  }
  auto detect(uintptr_t inputSrc, uintptr_t inputPoints, int border) {
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    auto points = reinterpret_cast<KeyPoints*>(inputPoints);

    auto obj = detect_internal(src, points, border);

    emscripten::val outObj = emscripten::val::object();
    emscripten::val pointsArr = emscripten::val::array();
    KPoint_t pt;
    for (auto i = 0; i < obj.pts.kpoints.size(); i++) {
      pt.x = obj.pts.kpoints[i].x;
      pt.y = obj.pts.kpoints[i].y;
      pt.level = obj.pts.kpoints[i].level;
      pt.score = obj.pts.kpoints[i].score;
      pt.angle = obj.pts.kpoints[i].angle;
      pointsArr.call<void>("push", pt);
    }
    outObj.set("count", obj.count);
    outObj.set("points", pointsArr);

    return outObj;
  }
  // getters and setters
  auto get_laplacian_threshold() const { return laplacian_threshold; };
  auto get_min_eigen_value_threshold() const {
    return min_eigen_value_threshold;
  }
  auto set_laplacian_threshold(int laplacian_value) {
    laplacian_threshold = laplacian_value;
  }
  auto set_min_eigen_value_threshold(int threshold_value) {
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