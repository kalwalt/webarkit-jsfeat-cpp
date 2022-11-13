#ifndef IMGPROC_H
#define IMGPROC_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#endif
#include <cache/Cache.h>
#include <jslog/jslog.h>
#include <math/math.h>
#include <matrix_smart/matrix_smart.h>
#include <matrix_t/matrix_t.h>
#include <node_utils/functions.h>
#include <types/types.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <type_traits>
#include <vector>

namespace jsfeat {

typedef struct {
  int si, di;
  unsigned int alpha;
} jsfeat_int_alpha;

#define jsfeat_clamp(x, a, b)               \
  ({                                        \
    typeof(a) _a = (a);                     \
    typeof(b) _b = (b);                     \
    typeof(x) _x = (x);                     \
    (_x < _a) ? _a : ((_x > _b) ? _b : _x); \
  })

class Imgproc : public Math {
 public:
  void grayscale_m(uintptr_t src, int w, int h, uintptr_t dst, int code) {
    auto ptrSrc = reinterpret_cast<Matrix_t*>(src);
    auto ptrDst = reinterpret_cast<Matrix_t*>(dst);
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

    if (ptrSrc->u8.empty()) {
      JSLOGe("vector is empty");
    }

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        ptrDst->u8[jr] =
            (ptrSrc->u8[ir] * coeff_r + ptrSrc->u8[ir + 1] * coeff_g +
             ptrSrc->u8[ir + 2] * coeff_b + 8192) >>
            14;
        ptrDst->u8[jr + 1] =
            (ptrSrc->u8[ir + cn] * coeff_r + ptrSrc->u8[ir + cn + 1] * coeff_g +
             ptrSrc->u8[ir + cn + 2] * coeff_b + 8192) >>
            14;
        ptrDst->u8[jr + 2] = (ptrSrc->u8[ir + cn2] * coeff_r +
                              ptrSrc->u8[ir + cn2 + 1] * coeff_g +
                              ptrSrc->u8[ir + cn2 + 2] * coeff_b + 8192) >>
                             14;
        ptrDst->u8[jr + 3] = (ptrSrc->u8[ir + cn3] * coeff_r +
                              ptrSrc->u8[ir + cn3 + 1] * coeff_g +
                              ptrSrc->u8[ir + cn3 + 2] * coeff_b + 8192) >>
                             14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->u8[jr] =
            (ptrSrc->u8[ir] * coeff_r + ptrSrc->u8[ir + 1] * coeff_g +
             ptrSrc->u8[ir + 2] * coeff_b + 8192) >>
            14;
      }
    }
  };

#ifdef __EMSCRIPTEN__
  void grayscale(emscripten::val inputSrc, int w, int h, uintptr_t dst,
                 int code) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
#else
  void grayscale(u_char* src, int w, int h, uintptr_t dst, int code) {
#endif
    auto ptrDst = reinterpret_cast<Matrix_t*>(dst);
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
        ptrDst->u8[jr] = (src[ir] * coeff_r + src[ir + 1] * coeff_g +
                          src[ir + 2] * coeff_b + 8192) >>
                         14;
        ptrDst->u8[jr + 1] =
            (src[ir + cn] * coeff_r + src[ir + cn + 1] * coeff_g +
             src[ir + cn + 2] * coeff_b + 8192) >>
            14;
        ptrDst->u8[jr + 2] =
            (src[ir + cn2] * coeff_r + src[ir + cn2 + 1] * coeff_g +
             src[ir + cn2 + 2] * coeff_b + 8192) >>
            14;
        ptrDst->u8[jr + 3] =
            (src[ir + cn3] * coeff_r + src[ir + cn3 + 1] * coeff_g +
             src[ir + cn3 + 2] * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        ptrDst->u8[jr] = (src[ir] * coeff_r + src[ir + 1] * coeff_g +
                          src[ir + 2] * coeff_b + 8192) >>
                         14;
      }
    }
  };
  void grayscale_rgba_standard(Array<u_char> src, int w, int h, Matrix_t* dst, int colorType) {
    int cn;
    switch (colorType) {
      case ColorsSpace::RGBA:
        cn = 4;
        break;
      case ColorsSpace::RGB:
        cn = 3;
        break;
      case ColorsSpace::GRAY:
         #ifdef DEBUG_EM
          throw "Grayscale input is not allwed with grayscale_rgba !";
        #else
          JSLOGe("Grayscale input is not allowed with grayscale_rgba !");
        #endif
        break;
      default:
        cn = 4;
    }
    // code from jsartoolkit5
    auto videosize = w * h;
    auto q = 0;
    u_char r;
    u_char g;
    u_char b;
    for (auto p = 0; p < videosize; p++) {
      r = src[q + 0], g = src[q + 1], b = src[q + 2];
      // https://stackoverflow.com/a/596241/5843642
      dst->u8.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += cn;
    }
  }
  template <typename Matrix_class>
  void grayscale_rgba_internal(Array<u_char> src, int w, int h, std::shared_ptr<Matrix_class> dst, int colorType) {
    static_assert(
        std::is_base_of<Matrix_smart, Matrix_class>::value ||
            std::is_base_of<Matrix_t, Matrix_class>::value,
        "Matrix_class must inherit from Matrix_smart or Matrix_t class");
    int cn;
    switch (colorType) {
      case ColorsSpace::RGBA:
        cn = 4;
        break;
      case ColorsSpace::RGB:
        cn = 3;
        break;
      case ColorsSpace::GRAY:
        #ifdef DEBUG_EM
          throw "Grayscale input is not allowed with grayscale_rgba !";
        #else
          JSLOGe("Grayscale input is not allowed with grayscale_rgba !");
        #endif
        break;
      default:
        cn = 4;
    }
    // code from jsartoolkit5
    auto videosize = w * h;
    auto q = 0;
    u_char r;
    u_char g;
    u_char b;
    for (auto p = 0; p < videosize; p++) {
      r = src[q + 0], g = src[q + 1], b = src[q + 2];
      // https://stackoverflow.com/a/596241/5843642
      dst->u8.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += cn;
    }
  }
#ifdef __EMSCRIPTEN__
  void grayscale_rgba(emscripten::val inputSrc, int w, int h, Matrix_t* dst, int colorType) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
    grayscale_rgba_standard(src, w, h, dst, colorType);
  }
  void grayscale_rgba(emscripten::val inputSrc, int w, int h,
                      std::shared_ptr<Matrix_smart> dst, int colorType) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
    grayscale_rgba_internal<Matrix_smart>(src, w, h, dst, colorType);
  }
  void grayscale_rgba(emscripten::val inputSrc, int w, int h,
                      std::shared_ptr<Matrix_t> dst, int colorType) {
    auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
    grayscale_rgba_internal<Matrix_t>(src, w, h, dst, colorType);
  }
#endif
  void pyrdown(uintptr_t inputSrc, uintptr_t inputDst, int sx, int sy) {
    auto dst = reinterpret_cast<Matrix_t*>(inputDst);
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    if (!sx) {
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

    // u_char* src_d = src->dt->u8.data();
    // u_char* dst_d = dst->dt->u8.data();

    for (y = 0; y < _h2; ++y) {
      sline = sptr;
      dline = dptr;
      for (x = 0; x <= _w2 - 2; x += 2, dline += 2, sline += 4) {
        dst->u8[dline] = (src->u8[sline] + src->u8[sline + 1] +
                          src->u8[sline + w] + src->u8[sline + w + 1] + 2) >>
                         2;
        dst->u8[dline + 1] =
            (src->u8[sline + 2] + src->u8[sline + 3] + src->u8[sline + w + 2] +
             src->u8[sline + w + 3] + 2) >>
            2;
      }
      for (; x < _w2; ++x, ++dline, sline += 2) {
        dst->u8[dline] = (src->u8[sline] + src->u8[sline + 1] +
                          src->u8[sline + w] + src->u8[sline + w + 1] + 2) >>
                         2;
      }
      sptr += w << 1;
      dptr += w2;
    }
  };
  void pyrdown_internal(Matrix_t* src, Matrix_t* dst, int sx = 0, int sy = 0) {
    if (!sx) {
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

    u_char* src_d = src->u8.data();
    u_char* dst_d = dst->u8.data();

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

  void equalize_histogram_internal(Matrix_t* src, Matrix_t* dst) {
    auto w = src->cols, h = src->rows;

    dst->resize(w, h, src->channel);

    /* max_val must be 255 if we use unsigned char as input, this should be
       changed if we use another input type (short, int...)
     */
    const auto max_val = 255;

    int total = w * h;
    auto n_bins = max_val + 1;

    // Compute histogram
    auto hist = cache.put_buffer(n_bins, Types::S32_t | Types::C1_t);
    for (int i = 0; i < total; ++i) {
      hist->i32[src->u8[i]]++;
    }

    // Build LUT from cumulative histrogram

    // Find first non-zero bin
    auto i = 0;
    while (!hist->i32[i]) ++i;

    if (hist->i32[i] == total) {
      for (int j = 0; j < total; ++j) {
        src->u8[j] = i;
      }
      return;
    }

    // Compute scale
    float scale = (n_bins - 1.f) / (total - hist->i32[i]);

    // Initialize lut
    auto lut = cache.put_buffer(n_bins, Types::S32_t | Types::C1_t);
    i++;

    auto sum = 0;
    for (; i < hist->i32.size(); ++i) {
      sum += hist->i32[i];
      // the value is saturated in range [0, max_val]
      lut->i32[i] =
          std::max(0, std::min(int(std::round(sum * scale)), max_val));
    }

    // Apply equalization
    for (int i = 0; i < total; ++i) {
      dst->u8[i] = lut->i32[src->u8[i]];
    }
    // to do: buffer should be put down the head...
  };

  void equalize_histogram(uintptr_t inputSrc, uintptr_t inputDst) {
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    auto dst = reinterpret_cast<Matrix_t*>(inputDst);
    this->equalize_histogram_internal(src, dst);
  }
  void warp_affine_internal(Matrix_t* src, Matrix_t* dst, Matrix_t* transform,
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
    float* td = transform->f32.data();
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

          p0 = src->u8[off] + a * (src->u8[off + 1] - src->u8[off]);
          p1 = src->u8[off + src_width] +
               a * (src->u8[off + src_width + 1] - src->u8[off + src_width]);

          dst->u8[dptr] = p0 + b * (p1 - p0);
        } else
          dst->u8[dptr] = fill_value;
      }
    }
  }
  void warp_affine(uintptr_t src, uintptr_t dst, uintptr_t transform,
                   int fill_value) {
    auto ptrSrc = reinterpret_cast<Matrix_t*>(src);
    auto ptrDst = reinterpret_cast<Matrix_t*>(dst);
    auto ptrTransform = reinterpret_cast<Matrix_t*>(transform);
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
    float* td = ptrTransform->f32.data();
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

          p0 = ptrSrc->u8[off] + a * (ptrSrc->u8[off + 1] - ptrSrc->u8[off]);
          p1 = ptrSrc->u8[off + src_width] +
               a * (ptrSrc->u8[off + src_width + 1] -
                    ptrSrc->u8[off + src_width]);

          ptrDst->u8[dptr] = p0 + b * (p1 - p0);
        } else
          ptrDst->u8[dptr] = fill_value;
      }
    }
  }
  void resample(uintptr_t inputSrc, uintptr_t inputDst, int nw, int nh) {
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    auto dst = reinterpret_cast<Matrix_t*>(inputDst);
    int h = src->rows, w = src->cols;
    if (h > nh && w > nw) {
      dst->resize(nw, nh, src->channel);
      // using the fast alternative (fix point scale, 0x100 to avoid overflow)
      if (src->type & Types::U8_t && dst->type & Types::U8_t &&
          h * w / (nh * nw) < 0x100) {
        _resample_u8(src, dst, nw, nh);
      } else {
        _resample(src, dst, nw, nh);
      }
    }
  };
  void resample(Matrix_t* src, Matrix_t* dst, int nw, int nh) {
    int h = src->rows, w = src->cols;
    if (h > nh && w > nw) {
      dst->resize(nw, nh, src->channel);
      // using the fast alternative (fix point scale, 0x100 to avoid overflow)
      if (src->type & Types::U8_t && dst->type & Types::U8_t &&
          h * w / (nh * nw) < 0x100) {
        _resample_u8(src, dst, nw, nh);
      } else {
        _resample(src, dst, nw, nh);
      }
    }
  };
  void gaussian_blur(uintptr_t inputSrc, uintptr_t inputDst, int kernel_size,
                     float sigma) {
    auto src = reinterpret_cast<Matrix_t*>(inputSrc);
    auto dst = reinterpret_cast<Matrix_t*>(inputDst);
    gaussian_blur_internal(src, dst, kernel_size, sigma);
  };
  void gaussian_blur_internal(Matrix_t* src, Matrix_t* dst, int kernel_size,
                              float sigma) {
    if (!sigma) {
      sigma = 0.0;
    }
    if (!kernel_size) {
      kernel_size = 0;
    }
    kernel_size =
        kernel_size == 0
            ? (std::max(1, (int)(4.0 * sigma + 1.0 - 1e-8)) * 2 + 1) | 0
            : kernel_size;
    int half_kernel = kernel_size >> 1;
    int w = src->cols, h = src->rows;
    int data_type = src->type;
    int is_u8 = data_type & Types::U8_t;

    dst->resize(w, h, src->channel);

    auto src_d = src->u8;
    auto dst_d = dst->u8;
    int buf_sz = (kernel_size + std::max(h, w)) | 0;

    if (is_u8) {
      /* We could use the cache, but we need to provide the appropiate values to
         the functions, this will be tested in another moment.*/
      // auto buf = cache.put_buffer(buf_sz<<2, Types::S32_t | Types::C1_t);
      // auto filter = cache.put_buffer(kernel_size<<2, Types::S32_t |
      // Types::C1_t);
      Array<int> buf(buf_sz << 2, 0);
      Array<int> filter(kernel_size << 2, 0);
      // From Math class
      get_gaussian_kernel<int>(kernel_size, sigma, filter, data_type);
      _convol_u8<int, int>(buf, src_d, dst_d, w, h, filter, kernel_size,
                           half_kernel);
    } else if (data_type & Types::S32_t) {
      // auto buf = cache.put_buffer(buf_sz<<2, Types::S32_t | Types::C1_t);
      // auto filter = cache.put_buffer(kernel_size<<2, Types::F32_t |
      // Types::C1_t);
      Array<int> buf(buf_sz << 2, 0);
      Array<float> filter(kernel_size << 2, 0.0);
      // From Math class
      get_gaussian_kernel<float>(kernel_size, sigma, filter, data_type);
      _convol<int, float>(buf, src_d, dst_d, w, h, filter, kernel_size,
                          half_kernel);
    } else {
      // auto buf = cache.put_buffer(buf_sz<<2, Types::F32_t | Types::C1_t);
      // auto filter = cache.put_buffer(kernel_size<<2, Types::F32_t |
      // Types::C1_t);
      Array<float> buf(buf_sz << 2, 0.0);
      Array<float> filter(kernel_size << 2, 0.0);
      // From Math class
      get_gaussian_kernel<float>(kernel_size, sigma, filter, data_type);
      _convol<float, float>(buf, src_d, dst_d, w, h, filter, kernel_size,
                            half_kernel);
    }
  };

 private:
  // a is src matrix, b is dst matrix
  void _resample_u8(Matrix_t* src, Matrix_t* dst, int nw, int nh) {
    int h = src->rows, w = src->cols;
    assert(src->cols > 0 && dst->cols > 0);
    Array<jsfeat_int_alpha> xofs(src->cols * 2);
    int ch = jsfeat_clamp(get_channel(src->type), 1, 4);
    double scale_x = (double)src->cols / dst->cols;
    double scale_y = (double)src->rows / dst->rows;
    // double scale = 1.f / (scale_x * scale_y);
    unsigned int inv_scale_256 = (int)(scale_x * scale_y * 0x10000);
    int dx, dy, sx, sy, i, k, a, b;
    for (dx = 0, k = 0; dx < dst->cols; dx++) {
      double fsx1 = dx * scale_x, fsx2 = fsx1 + scale_x;
      int sx1 = (int)(fsx1 + 1.0 - 1e-6), sx2 = (int)(fsx2);
      sx1 = std::min(sx1, src->cols - 1);
      sx2 = std::min(sx2, src->cols - 1);

      if (sx1 > fsx1) {
        xofs[k].di = dx * ch;
        xofs[k].si = (sx1 - 1) * ch;
        xofs[k++].alpha = (unsigned int)((sx1 - fsx1) * 0x100);
      }

      for (sx = sx1; sx < sx2; sx++) {
        xofs[k].di = dx * ch;
        xofs[k].si = sx * ch;
        xofs[k++].alpha = 256;
      }

      if (fsx2 - sx2 > 1e-3) {
        xofs[k].di = dx * ch;
        xofs[k].si = sx2 * ch;
        xofs[k++].alpha = (unsigned int)((fsx2 - sx2) * 256);
      }
    }
    int xofs_count = k;
    Array<u_int> buf(dst->cols * ch);
    Array<u_int> sum(dst->cols * ch);
    for (dx = 0; dx < dst->cols * ch; dx++) {
      buf[dx] = sum[dx] = 0;
    }
    dy = 0;
    for (sy = 0; sy < src->rows; sy++) {
      a = w * sy;
      Array<u_char> a_ptr = dst->u8;
      for (k = 0; k < xofs_count; k++) {
        int dxn = xofs[k].di;
        unsigned int alpha = xofs[k].alpha;
        for (i = 0; i < ch; i++)
          buf[dxn + i] += a_ptr[a + xofs[k].si + i] * alpha;
      }
      if ((dy + 1) * scale_y <= sy + 1 || sy == src->rows - 1) {
        unsigned int beta =
            (int)((std::max(sy + 1 - (dy + 1) * scale_y, 0.)) * 256);
        unsigned int beta1 = 256 - beta;
        b = nw * dy;
        Array<u_char> b_ptr = dst->u8;  // + b->step  * dy;
        if (beta <= 0) {
          for (dx = 0; dx < dst->cols * ch; dx++) {
            b_ptr[b + dx] =
                jsfeat_clamp(sum[dx] + buf[dx] * 256 / inv_scale_256, 0, 255);
            sum[dx] = buf[dx] = 0;
          }
        } else {
          for (dx = 0; dx < dst->cols * ch; dx++) {
            b_ptr[b + dx] = jsfeat_clamp(
                (sum[dx] + buf[dx] * beta1) / inv_scale_256, 0, 255);
            sum[dx] = buf[dx] * beta;
            buf[dx] = 0;
          }
        }
        dy++;
      } else {
        for (dx = 0; dx < dst->cols * ch; dx++) {
          sum[dx] += buf[dx] * 256;
          buf[dx] = 0;
        }
      }
    }
  }

  void _resample(Matrix_t* src, Matrix_t* dst, int nw, int nh) {
    // to be implemented!
  }
  template <typename Buffer, typename Filter>
  void _convol_u8(Array<Buffer>& buf, Array<u_char>& src_d,
                  Array<u_char>& dst_d, int w, int h, Array<Filter>& filter,
                  int kernel_size, int half_kernel) {
    auto i = 0, j = 0, k = 0, sp = 0, dp = 0, sum = 0, sum1 = 0, sum2 = 0,
         sum3 = 0;
    auto f0 = filter[0];
    auto fk = 0.0;
    auto w2 = w << 1, w3 = w * 3, w4 = w << 2;
    // hor pass
    for (; i < h; ++i) {
      sum = src_d[sp];
      for (j = 0; j < half_kernel; ++j) {
        buf[j] = sum;
      }
      for (j = 0; j <= w - 2; j += 2) {
        buf[j + half_kernel] = src_d[sp + j];
        buf[j + half_kernel + 1] = src_d[sp + j + 1];
      }
      for (; j < w; ++j) {
        buf[j + half_kernel] = src_d[sp + j];
      }
      sum = src_d[sp + w - 1];
      for (j = w; j < half_kernel + w; ++j) {
        buf[j + half_kernel] = sum;
      }
      for (j = 0; j <= w - 4; j += 4) {
        sum = buf[j] * f0, sum1 = buf[j + 1] * f0, sum2 = buf[j + 2] * f0,
        sum3 = buf[j + 3] * f0;
        for (k = 1; k < kernel_size; ++k) {
          fk = filter[k];
          sum += buf[k + j] * fk;
          sum1 += buf[k + j + 1] * fk;
          sum2 += buf[k + j + 2] * fk;
          sum3 += buf[k + j + 3] * fk;
        }
        dst_d[dp + j] = std::min(sum >> 8, 255);
        dst_d[dp + j + 1] = std::min(sum1 >> 8, 255);
        dst_d[dp + j + 2] = std::min(sum2 >> 8, 255);
        dst_d[dp + j + 3] = std::min(sum3 >> 8, 255);
      }
      for (; j < w; ++j) {
        sum = buf[j] * f0;
        for (k = 1; k < kernel_size; ++k) {
          sum += buf[k + j] * filter[k];
        }
        dst_d[dp + j] = std::min(sum >> 8, 255);
      }
      sp += w;
      dp += w;
    }

    // vert pass
    for (i = 0; i < w; ++i) {
      sum = dst_d[i];
      for (j = 0; j < half_kernel; ++j) {
        buf[j] = sum;
      }
      k = i;
      for (j = 0; j <= h - 2; j += 2, k += w2) {
        buf[j + half_kernel] = dst_d[k];
        buf[j + half_kernel + 1] = dst_d[k + w];
      }
      for (; j < h; ++j, k += w) {
        buf[j + half_kernel] = dst_d[k];
      }
      sum = dst_d[(h - 1) * w + i];
      for (j = h; j < half_kernel + h; ++j) {
        buf[j + half_kernel] = sum;
      }
      dp = i;
      for (j = 0; j <= h - 4; j += 4, dp += w4) {
        sum = buf[j] * f0, sum1 = buf[j + 1] * f0, sum2 = buf[j + 2] * f0,
        sum3 = buf[j + 3] * f0;
        for (k = 1; k < kernel_size; ++k) {
          fk = filter[k];
          sum += buf[k + j] * fk;
          sum1 += buf[k + j + 1] * fk;
          sum2 += buf[k + j + 2] * fk;
          sum3 += buf[k + j + 3] * fk;
        }
        dst_d[dp] = std::min(sum >> 8, 255);
        dst_d[dp + w] = std::min(sum1 >> 8, 255);
        dst_d[dp + w2] = std::min(sum2 >> 8, 255);
        dst_d[dp + w3] = std::min(sum3 >> 8, 255);
      }
      for (; j < h; ++j, dp += w) {
        sum = buf[j] * f0;
        for (k = 1; k < kernel_size; ++k) {
          sum += buf[k + j] * filter[k];
        }
        dst_d[dp] = std::min(sum >> 8, 255);
      }
    }
  }
  template <typename Buffer, typename Filter>
  void _convol(Array<Buffer>& buf, Array<u_char>& src_d, Array<u_char>& dst_d,
               int w, int h, Array<Filter>& filter, int kernel_size,
               int half_kernel) {
    auto i = 0, j = 0, k = 0, sp = 0, dp = 0;
    auto sum = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    auto f0 = filter[0];
    auto fk = 0.0;
    auto w2 = w << 1, w3 = w * 3, w4 = w << 2;
    // hor pass
    for (; i < h; ++i) {
      sum = src_d[sp];
      for (j = 0; j < half_kernel; ++j) {
        buf[j] = sum;
      }
      for (j = 0; j <= w - 2; j += 2) {
        buf[j + half_kernel] = src_d[sp + j];
        buf[j + half_kernel + 1] = src_d[sp + j + 1];
      }
      for (; j < w; ++j) {
        buf[j + half_kernel] = src_d[sp + j];
      }
      sum = src_d[sp + w - 1];
      for (j = w; j < half_kernel + w; ++j) {
        buf[j + half_kernel] = sum;
      }
      for (j = 0; j <= w - 4; j += 4) {
        sum = buf[j] * f0, sum1 = buf[j + 1] * f0, sum2 = buf[j + 2] * f0,
        sum3 = buf[j + 3] * f0;
        for (k = 1; k < kernel_size; ++k) {
          fk = filter[k];
          sum += buf[k + j] * fk;
          sum1 += buf[k + j + 1] * fk;
          sum2 += buf[k + j + 2] * fk;
          sum3 += buf[k + j + 3] * fk;
        }
        dst_d[dp + j] = sum;
        dst_d[dp + j + 1] = sum1;
        dst_d[dp + j + 2] = sum2;
        dst_d[dp + j + 3] = sum3;
      }
      for (; j < w; ++j) {
        sum = buf[j] * f0;
        for (k = 1; k < kernel_size; ++k) {
          sum += buf[k + j] * filter[k];
        }
        dst_d[dp + j] = sum;
      }
      sp += w;
      dp += w;
    }

    // vert pass
    for (i = 0; i < w; ++i) {
      sum = dst_d[i];
      for (j = 0; j < half_kernel; ++j) {
        buf[j] = sum;
      }
      k = i;
      for (j = 0; j <= h - 2; j += 2, k += w2) {
        buf[j + half_kernel] = dst_d[k];
        buf[j + half_kernel + 1] = dst_d[k + w];
      }
      for (; j < h; ++j, k += w) {
        buf[j + half_kernel] = dst_d[k];
      }
      sum = dst_d[(h - 1) * w + i];
      for (j = h; j < half_kernel + h; ++j) {
        buf[j + half_kernel] = sum;
      }
      dp = i;
      for (j = 0; j <= h - 4; j += 4, dp += w4) {
        sum = buf[j] * f0, sum1 = buf[j + 1] * f0, sum2 = buf[j + 2] * f0,
        sum3 = buf[j + 3] * f0;
        for (k = 1; k < kernel_size; ++k) {
          fk = filter[k];
          sum += buf[k + j] * fk;
          sum1 += buf[k + j + 1] * fk;
          sum2 += buf[k + j + 2] * fk;
          sum3 += buf[k + j + 3] * fk;
        }
        dst_d[dp] = sum;
        dst_d[dp + w] = sum1;
        dst_d[dp + w2] = sum2;
        dst_d[dp + w3] = sum3;
      }
      for (; j < h; ++j, dp += w) {
        sum = buf[j] * f0;
        for (k = 1; k < kernel_size; ++k) {
          sum += buf[k + j] * filter[k];
        }
        dst_d[dp] = sum;
      }
    }
  }

};  // class imgproc

}  // namespace jsfeat

#endif