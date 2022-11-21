#ifndef DETECTORS_H
#define DETECTORS_H

#include <keypoint_t/keypoint_t.h>
#include <keypoints/keypoints.h>
#include <keypoints_filter/keypoints_filter.h>
#include <math/math.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <yape06/yape06.h>

namespace jsfeat {

namespace detectors {

class Detectors : public Yape06, public Math, public KeyPointsFilter {
 public:
  Detectors() {}
  ~Detectors() {}

  int detect_keypoints(Matrix_t* img, KeyPoints corners, int max_allowed) {
    // detect features
    auto kpc = detect_internal(img, &corners, 17);
    auto count = kpc.count;
    // sort by score and reduce the count if needed
    if (count > max_allowed) {
      // qsort_internal<KeyPoint_t, bool>(corners.kpoints, 0, count - 1, [](KeyPoint_t i, KeyPoint_t j){return (i.score < j.score);});
      retainBest(corners.kpoints, count);
      count = max_allowed;
    }

    // calculate dominant orientation for each keypoint
    for (auto i = 0; i < count; ++i) {
      corners.kpoints[i].angle = ic_angle(img, corners.kpoints[i].x, corners.kpoints[i].y);
    }

    return count;
  }

 private:
  // function(a, b) { return (b.score < a.score); }
  // bool myfunction(KeyPoint_t i, KeyPoint_t j) { return (i.score < j.score); }
  // central difference using image moments to find dominant orientation
  // var u_max = new Int32Array([15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 9, 8, 6, 3, 0]);
  float ic_angle(Matrix_t* img, int px, int py) {
    Array<u_int> u_max{15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 9, 8, 6, 3, 0};
    auto half_k = 15;  // half patch size
    auto m_01 = 0, m_10 = 0;
    auto src = img->u8;
    auto step = img->get_cols();
    auto u = 0, v = 0, center_off = (py * step + px) | 0;
    auto v_sum = 0, d = 0, val_plus = 0, val_minus = 0;

    // Treat the center line differently, v=0
    for (u = -half_k; u <= half_k; ++u)
      m_10 += u * src[center_off + u];

    // Go line by line in the circular patch
    for (v = 1; v <= half_k; ++v) {
      // Proceed over the two lines
      v_sum = 0;
      d = u_max[v];
      for (u = -d; u <= d; ++u) {
        val_plus = src[center_off + u + v * step];
        val_minus = src[center_off + u - v * step];
        v_sum += (val_plus - val_minus);
        m_10 += u * (val_plus + val_minus);
      }
      m_01 += v * v_sum;
    }

    return std::atan2(m_01, m_10);
  }
};

}  // namespace detectors

}  // namespace jsfeat

#endif