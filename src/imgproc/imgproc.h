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
  void grayscale(emscripten::val src, int w, int h, uintptr_t dst, int code) {
    auto ptrSrc = emscripten::convertJSArrayToNumberVector<u_char>(src);
#else
  void grayscale(uintptr_t src, int w, int h, uintptr_t dst, int code) {
    auto ptrSrc = reinterpret_cast<u_char*>(src);
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
            (ptrSrc[ir] * coeff_r + ptrSrc[ir + 1] * coeff_g +
             ptrSrc[ir + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 1) =
            (ptrSrc[ir + cn] * coeff_r +
             ptrSrc[ir + cn + 1] * coeff_g +
             ptrSrc[ir + cn + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 2) =
            (ptrSrc[ir + cn2] * coeff_r +
             ptrSrc[ir + cn2 + 1] * coeff_g +
             ptrSrc[ir + cn2 + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 3) =
            (ptrSrc[ir + cn3] * coeff_r +
             ptrSrc[ir + cn3 + 1] * coeff_g +
             ptrSrc[ir + cn3 + 2] * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->dt->u8.at(jr) =
            (ptrSrc[ir] * coeff_r + ptrSrc[ir + 1] * coeff_g +
             ptrSrc[ir + 2] * coeff_b + 8192) >>
            14;
      }
    }
  };
};
} // namespace jsfeat

#endif