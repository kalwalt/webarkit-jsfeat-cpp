#ifndef IMGPROC_H
#define IMGPROC_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#endif
#include <jslog/jslog.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>

namespace jsfeat {

class imgproc {
public:
  void grayscale_m(uintptr_t src, int w, int h, uintptr_t dst, int code) {
    auto ptrSrc = reinterpret_cast<matrix_t *>(src);
    auto ptrDst = reinterpret_cast<matrix_t *>(dst);
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
        ptrDst->dt->u8.at(jr) = (ptrSrc->dt->u8.at(ir) * coeff_r +
                                 ptrSrc->dt->u8.at(ir + 1) * coeff_g +
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
        ptrDst->dt->u8.at(jr) = (ptrSrc->dt->u8.at(ir) * coeff_r +
                                 ptrSrc->dt->u8.at(ir + 1) * coeff_g +
                                 ptrSrc->dt->u8.at(ir + 2) * coeff_b + 8192) >>
                                14;
      }
    }
  };

#ifdef __EMSCRIPTEN__
  void grayscale(emscripten::val inputSrc, int w, int h, uintptr_t dst,
                 int code) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
#else
  void grayscale(u_char *src, int w, int h, uintptr_t dst, int code) {
#endif
    auto ptrDst = reinterpret_cast<matrix_t *>(dst);
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
        ptrDst->dt->u8.at(jr) = (src[ir] * coeff_r + src[ir + 1] * coeff_g +
                                 src[ir + 2] * coeff_b + 8192) >>
                                14;
        ptrDst->dt->u8.at(jr + 1) =
            (src[ir + cn] * coeff_r + src[ir + cn + 1] * coeff_g +
             src[ir + cn + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 2) =
            (src[ir + cn2] * coeff_r + src[ir + cn2 + 1] * coeff_g +
             src[ir + cn2 + 2] * coeff_b + 8192) >>
            14;
        ptrDst->dt->u8.at(jr + 3) =
            (src[ir + cn3] * coeff_r + src[ir + cn3 + 1] * coeff_g +
             src[ir + cn3 + 2] * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->dt->u8.at(jr) = (src[ir] * coeff_r + src[ir + 1] * coeff_g +
                                 src[ir + 2] * coeff_b + 8192) >>
                                14;
      }
    }
  };
  void pyrdown(uintptr_t inputSrc, uintptr_t inputDst, int sx, int sy) {
    auto dst = reinterpret_cast<matrix_t *>(inputDst);
    auto src = reinterpret_cast<matrix_t *>(inputSrc);
    if (!sx ) {
      sx = 0;
    }
    if (!sy) {
      sy = 0;
    }

    int w = src->cols, h = src->rows;
    int w2 = w >> 1, h2 = h >> 1;
    int _w2 = w2 - (sx << 1), _h2 = h2 - (sy << 1);
    int x = 0, y = 0, sptr = sx + sy * w, sline = 0, dptr = 0, dline = 0;

    dst->resize(w2, h2, src->channel);

    //u_char* src_d = src->dt->u8.data();
    //u_char* dst_d = dst->dt->u8.data();

    for (y = 0; y < _h2; ++y) {
      sline = sptr;
      dline = dptr;
      for (x = 0; x <= _w2 - 2; x += 2, dline += 2, sline += 4) {
        dst->dt->u8.at(dline) = (src->dt->u8.at(sline) + src->dt->u8.at(sline + 1) + src->dt->u8.at(sline + w) +
                        src->dt->u8.at(sline + w + 1) + 2) >>
                       2;
        dst->dt->u8.at(dline + 1) = (src->dt->u8.at(sline + 2) + src->dt->u8.at(sline + 3) +
                            src->dt->u8.at(sline + w + 2) + src->dt->u8.at(sline + w + 3) + 2) >>
                           2;
      }
      for (; x < _w2; ++x, ++dline, sline += 2) {
        dst->dt->u8.at(dline) = (src->dt->u8.at(sline) + src->dt->u8.at(sline + 1) + src->dt->u8.at(sline + w) +
                        src->dt->u8.at(sline + w + 1) + 2) >>
                       2;
      }
      sptr += w << 1;
      dptr += w2;
    }
  };
  void pyrdown_internal(matrix_t *src, matrix_t *dst, int sx=0, int sy=0) {
    if (!sx ) {
      sx = 0;
    }
    if (!sy) {
      sy = 0;
    }

    int w = src->cols, h = src->rows;
    int w2 = w >> 1, h2 = h >> 1;
    int _w2 = w2 - (sx << 1), _h2 = h2 - (sy << 1);
    int x = 0, y = 0, sptr = sx + sy * w, sline = 0, dptr = 0, dline = 0;

    dst->resize(w2, h2, src->channel);

    u_char* src_d = src->dt->u8.data();
    u_char* dst_d = dst->dt->u8.data();

    for (y = 0; y < _h2; ++y) {
      sline = sptr;
      dline = dptr;
      for (x = 0; x <= _w2 - 2; x += 2, dline += 2, sline += 4) {
        dst_d[dline] = (src_d[sline] + src_d[sline + 1] + src_d[sline + w] +
                        src_d[sline + w + 1] + 2) >>
                       2;
        dst_d[dline + 1] = (src_d[sline + 2] + src_d[sline + 3] +
                            src_d[sline + w + 2] + src_d[sline + w + 3] + 2) >>
                           2;
      }
      for (; x < _w2; ++x, ++dline, sline += 2) {
        dst_d[dline] = (src_d[sline] + src_d[sline + 1] + src_d[sline + w] +
                        src_d[sline + w + 1] + 2) >>
                       2;
      }
      sptr += w << 1;
      dptr += w2;
    }
  };
  void warp_affine_internal(matrix_t *src, matrix_t *dst, matrix_t *transform,
                            int fill_value) {
    if (!fill_value) {
      fill_value = 0;
    }
    int src_width = src->cols;
    int src_height = src->rows;
    int dst_width = dst->cols;
    int dst_height = dst->rows;
    int x = 0;
    int y = 0;
    int off = 0;
    int ixs = 0;
    int iys = 0;
    int xs = 0.0;
    int ys = 0.0;
    float a = 0.0;
    float b = 0.0;
    float p0 = 0.0;
    float p1 = 0.0;
    float *td = transform->dt->f32.data();
    float m00 = td[0];
    float m01 = td[1];
    float m02 = td[2];
    float m10 = td[3];
    float m11 = td[4];
    float m12 = td[5];
    for (int dptr = 0; y < dst_height; ++y) {
      xs = m01 * y + m02;
      ys = m11 * y + m12;
      for (x = 0; x < dst_width; ++x, ++dptr, xs += m00, ys += m10) {
        ixs = xs | 0;
        iys = ys | 0;

        if (ixs >= 0 && iys >= 0 && ixs < (src_width - 1) &&
            iys < (src_height - 1)) {
          a = xs - ixs;
          b = ys - iys;
          off = src_width * iys + ixs;

          p0 = src->dt->u8[off] + a * (src->dt->u8[off + 1] - src->dt->u8[off]);
          p1 = src->dt->u8[off + src_width] +
               a * (src->dt->u8[off + src_width + 1] -
                    src->dt->u8[off + src_width]);

          dst->dt->u8[dptr] = p0 + b * (p1 - p0);
        } else
          dst->dt->u8[dptr] = fill_value;
      }
    }
  }
  void warp_affine(uintptr_t src, uintptr_t dst, uintptr_t transform,
                   int fill_value) {
    auto ptrSrc = reinterpret_cast<matrix_t *>(src);
    auto ptrDst = reinterpret_cast<matrix_t *>(dst);
    auto ptrTransform = reinterpret_cast<matrix_t *>(transform);
    if (!fill_value) {
      fill_value = 0;
    }
    int src_width = ptrSrc->cols;
    int src_height = ptrSrc->rows;
    int dst_width = ptrDst->cols;
    int dst_height = ptrDst->rows;
    int x = 0;
    int y = 0;
    int off = 0;
    int ixs = 0;
    int iys = 0;
    int xs = 0.0;
    int ys = 0.0;
    float a = 0.0;
    float b = 0.0;
    float p0 = 0.0;
    float p1 = 0.0;
    float *td = ptrTransform->dt->f32.data();
    float m00 = td[0];
    float m01 = td[1];
    float m02 = td[2];
    float m10 = td[3];
    float m11 = td[4];
    float m12 = td[5];
    for (int dptr = 0; y < dst_height; ++y) {
      xs = m01 * y + m02;
      ys = m11 * y + m12;
      for (x = 0; x < dst_width; ++x, ++dptr, xs += m00, ys += m10) {
        ixs = xs | 0;
        iys = ys | 0;

        if (ixs >= 0 && iys >= 0 && ixs < (src_width - 1) &&
            iys < (src_height - 1)) {
          a = xs - ixs;
          b = ys - iys;
          off = src_width * iys + ixs;

          p0 = ptrSrc->dt->u8[off] +
               a * (ptrSrc->dt->u8[off + 1] - ptrSrc->dt->u8[off]);
          p1 = ptrSrc->dt->u8[off + src_width] +
               a * (ptrSrc->dt->u8[off + src_width + 1] -
                    ptrSrc->dt->u8[off + src_width]);

          ptrDst->dt->u8[dptr] = p0 + b * (p1 - p0);
        } else
          ptrDst->dt->u8[dptr] = fill_value;
      }
    }
  }
};

} // namespace jsfeat

#endif