#ifndef ORB_H
#define ORB_H

#include <emscripten/val.h>
#include <imgproc/imgproc.h>
#include <jslog/jslog.h>
#include <keypoint_t/keypoint_t.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>

#include <iostream>

#include "bit_pattern_31.h"

namespace jsfeat {

class Orb {
 public:
  Orb() {
    H = new Matrix_t(3, 3, Types::F32_t | Types::C1_t);
    H->allocate();
    patch_img = new Matrix_t(32, 32, Types::U8_t | Types::C1_t);
    patch_img->allocate();
  }

  void describe(uintptr_t inputSrc, emscripten::val inputCorners, int count, uintptr_t inputDescriptors) {
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    auto corners = emscripten::vecFromJSArray<KeyPoint_t>(inputCorners);
    auto descriptors = reinterpret_cast<Matrix_t*>(inputDescriptors);
    int DESCR_SIZE = 32;  // bytes;
    int i = 0, b = 0, px = 0.0, py = 0.0, angle = 0.0;
    int t0 = 0, t1 = 0, val = 0;
    // int img = src.data, w = src.cols, h = src.rows;
    u_char* patch_d = patch_img->u8.data();
    int patch_off = 16 * 32 + 16;  // center of patch
    int patt = 0;

    if (!(descriptors->get_type() & Types::U8_t)) {
      // relocate to U8 type
      descriptors->set_type(Types::U8_t);
      descriptors->set_cols(DESCR_SIZE);
      descriptors->set_rows(count);
      descriptors->set_channel_m(1);
      descriptors->allocate();
    } else {
      descriptors->resize(DESCR_SIZE, count, 1);
    }

    u_char* descr_d = descriptors->u8.data();
    int descr_off = 0;

    for (i = 0; i < count; ++i) {
      px = corners[i].x;
      py = corners[i].y;
      angle = corners[i].angle;

      rectify_patch(src, patch_img, angle, px, py, 32);

      // describe the patch
      patt = 0;
      for (b = 0; b < DESCR_SIZE; ++b) {
        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val = (t0 < t1) | 0;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 1;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 2;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 3;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 4;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 5;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 6;

        t0 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        t1 = patch_d[patch_off + bit_pattern_31_[patt + 1] * 32 +
                     bit_pattern_31_[patt]];
        patt += 2;
        val |= (t0 < t1) << 7;

        descr_d[descr_off + b] = val;
      }
      descr_off += DESCR_SIZE;
    }
  }

 private:
  Matrix_t* H;
  Matrix_t* patch_img;

  void rectify_patch(Matrix_t* src, Matrix_t* dst, float angle, int px, int py, int psize) {
    Imgproc* proc;

    float cosine = std::cos(angle);
    float sine = std::sin(angle);
    H->f32[0] = cosine;
    H->f32[1] = -sine;
    H->f32[2] = ((-cosine + sine) * psize * 0.5 + px);
    H->f32[3] = sine;
    H->f32[4] = cosine;
    H->f32[5] = ((-sine - cosine) * psize * 0.5 + py);

    proc->warp_affine_internal(src, dst, H, 128);
  };
};

}  // namespace jsfeat

#endif