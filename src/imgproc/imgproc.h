#ifndef IMGPROC_H
#define IMGPROC_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#endif
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <jslog/jslog.h>

namespace jsfeat {

class imgproc {
public:
  void grayscale_m(uintptr_t src, int w, int h, uintptr_t dst, int code) {
    auto ptrSrc = reinterpret_cast<matrix_t*>(src);
    auto ptrDst = reinterpret_cast<matrix_t*>(dst);
    // this is default image data representation in browser
    if (!code) {
      code = Colors::COLOR_RGBA2GRAY;
    }
    int videosize = w * h;
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;
    int ir = 0;
    int jr = 0;
    int coeff_r = 4899;
    int coeff_g = 9617;
    int coeff_b = 1868;
    int cn = 4;

    if (code == Colors::COLOR_BGRA2GRAY || code == Colors::COLOR_BGR2GRAY) {
      coeff_r = 1868;
      coeff_b = 4899;
    }
    if (code == Colors::COLOR_RGB2GRAY || code == Colors::COLOR_BGR2GRAY) {
      cn = 3;
    }
    int cn2 = cn << 1;
    int cn3 = (cn * 3) | 0;

    ptrDst->resize(w, h, 1);

    if (ptrSrc->dt->u8.empty()) {
      JSLOGe("vector is empty");
    }

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      // probably we can do this in javascript but not in C++
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        ptrDst->dt->u8.at(jr) =
            (ptrSrc->dt->u8.at(ir) * coeff_r + ptrSrc->dt->u8.at(ir + 1) * coeff_g +
             ptrSrc->dt->u8.at(ir + 2) * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 1) =
            (ptrSrc->dt->u8.at(ir + cn) * coeff_r +
             ptrSrc->dt->u8.at(ir + cn + 1) * coeff_g +
             ptrSrc->dt->u8.at(ir + cn + 2) * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 2) =
            (ptrSrc->dt->u8.at(ir + cn2) * coeff_r +
             ptrSrc->dt->u8.at(ir + cn2 + 1) * coeff_g +
             ptrSrc->dt->u8.at(ir + cn2 + 2) * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 3) =
            (ptrSrc->dt->u8.at(ir + cn3) * coeff_r +
             ptrSrc->dt->u8.at(ir + cn3 + 1) * coeff_g +
             ptrSrc->dt->u8.at(ir + cn3 + 2) * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->dt->u8.at(jr) =
            (ptrSrc->dt->u8.at(ir) * coeff_r + ptrSrc->dt->u8.at(ir + 1) * coeff_g +
             ptrSrc->dt->u8.at(ir + 2) * coeff_b + 8192) >>
            14;
      }
    }
  };

#ifdef __EMSCRIPTEN__
  void grayscale(emscripten::val inputSrc, int w, int h, uintptr_t dst, int code) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
#else
  void grayscale(u_char* src, int w, int h, uintptr_t dst, int code) {
#endif
    auto ptrDst = reinterpret_cast<matrix_t*>(dst);
    // this is default image data representation in browser
    if (!code) {
      code = Colors::COLOR_RGBA2GRAY;
    }
    int videosize = w * h;
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;
    int ir = 0;
    int jr = 0;
    int coeff_r = 4899;
    int coeff_g = 9617;
    int coeff_b = 1868;
    int cn = 4;

    if (code == Colors::COLOR_BGRA2GRAY || code == Colors::COLOR_BGR2GRAY) {
      coeff_r = 1868;
      coeff_b = 4899;
    }
    if (code == Colors::COLOR_RGB2GRAY || code == Colors::COLOR_BGR2GRAY) {
      cn = 3;
    }
    int cn2 = cn << 1;
    int cn3 = (cn * 3) | 0;

    ptrDst->resize(w, h, 1);

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        ptrDst->dt->u8.at(jr) =
            (src[ir] * coeff_r + src[ir + 1] * coeff_g +
             src[ir + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 1) =
            (src[ir + cn] * coeff_r +
             src[ir + cn + 1] * coeff_g +
             src[ir + cn + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 2) =
            (src[ir + cn2] * coeff_r +
             src[ir + cn2 + 1] * coeff_g +
             src[ir + cn2 + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 3) =
            (src[ir + cn3] * coeff_r +
             src[ir + cn3 + 1] * coeff_g +
             src[ir + cn3 + 2] * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->dt->u8.at(jr) =
            (src[ir] * coeff_r + src[ir + 1] * coeff_g +
             src[ir + 2] * coeff_b + 8192) >>
            14;
      }
    }
  };
};
} // namespace jsfeat

#endif