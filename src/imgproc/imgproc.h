#ifndef IMGPROC_H
#define IMGPROC_H

#include <iostream>
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <vector>

namespace jsfeat {

class imgproc {
public:
  int code;
  void grayscale_m(matrix_t *src, int w, int h, matrix_t *dst, int code) {
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
    std::cout << "cn3: " << cn3 << std::endl;

    dst->resize(w, h, 1);
    std::cout << dst->dt->u8.size() << std::endl;
    std::cout << src->dt->u8.size() << std::endl;
    if (src->dt->u8.empty()) {
      std::cout << "vector is empty" << std::endl;
    }
    // this should print a zero value
    std::cout << "value: " << (int)src->dt->u8.at(0) << std::endl;

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      // probably we can do this in javascript but not in C++
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        dst->dt->u8.at(jr) =
            (src->dt->u8.at(ir) * coeff_r + src->dt->u8.at(ir + 1) * coeff_g +
             src->dt->u8.at(ir + 2) * coeff_b + 8192) >>
            14;
        dst->dt->u8.at(jr + 1) =
            (src->dt->u8.at(ir + cn) * coeff_r + src->dt->u8.at(ir + cn + 1) * coeff_g +
             src->dt->u8.at(ir + cn + 2) * coeff_b + 8192) >>
            14;
        dst->dt->u8.at(jr + 2) = (src->dt->u8.at(ir + cn2) * coeff_r +
                              src->dt->u8.at(ir + cn2 + 1) * coeff_g +
                              src->dt->u8.at(ir + cn2 + 2) * coeff_b + 8192) >>
                             14;
        dst->dt->u8.at(jr + 3) = (src->dt->u8.at(ir + cn3) * coeff_r +
                              src->dt->u8.at(ir + cn3 + 1) * coeff_g +
                              src->dt->u8.at(ir + cn3 + 2) * coeff_b + 8192) >>
                             14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        dst->dt->u8.at(jr) =
            (src->dt->u8.at(ir) * coeff_r + src->dt->u8.at(ir + 1) * coeff_g +
             src->dt->u8.at(ir + 2) * coeff_b + 8192) >>
            14;
      }
    }
    // code from jsartoolkit5 to compare with the jsfeat version
    /*int q = 0;
    int r;
    int g;
    int b;

    for (int p = 0; p < videosize-1; p++) {
      r = src->u8.at(q + 0), g = src->u8.at(q + 1), b = src->u8.at(q + 2);
      // https://stackoverflow.com/a/596241/5843642
      dst->u8.at(p) = (r + r + r + b + g + g + g + g) >> 3;
      q += 4;
    }*/
    for (int i = 0; i < dst->dt->u8.size(); i++) {
      std::cout << "value dst: " << (int)dst->dt->u8.at(i) << std::endl;
    }
  };

  void grayscale(u_char *src, int w, int h, matrix_t *dst, int code) {
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
    std::cout << "cn3: " << cn3 << std::endl;

    // dst.resize(w, h, 1);
    // std::cout << "dst size is: " << dst.u8.size() << std::endl;
    //  this should print a zero value
    std::cout << "value: " << (int)src[0] << std::endl;

    // code from jsartoolkit5
    int q = 0;
    /*unsigned char r;
    unsigned char g;
    unsigned char b;*/

    int r;
    int g;
    int b;

    // std::cout << "data size: " << std::size(dst->data)  << std::endl;

    for (int p = 0; p < videosize; p++) {
      r = src[q + 0];
      g = src[q + 1];
      b = src[q + 2];
      std::cout << "p is: " << p << std::endl;
      dst->data[p] = (r + r + r + b + g + g + g + g) >> 3;
      // https://stackoverflow.com/a/596241/5843642
      // dst.u8.at(p) = (r + r + r + b + g + g + g + g) >> 3;
      q += 4;
    }
    /*for (int i = 0; i < dst.u8.size(); i++) {
      std::cout << "value dst: " << (int)dst.u8.at(i) << std::endl;
    }*/
  };

  void grayscale_s(u_char *src, int w, int h, Mat_t dst, int code) {
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
    std::cout << "cn3: " << cn3 << std::endl;

    // dst->resize(w, h, 1);
    std::cout << "dst size is: " << dst.data.size() << std::endl;
    // this should print a zero value
    std::cout << "value: " << (int)src[0] << std::endl;

    // code from jsartoolkit5
    int q = 0;
    int r;
    int g;
    int b;

    for (int p = 0; p < videosize; p++) {
      r = src[q + 0], g = src[q + 1], b = src[q + 2];
      std::cout << "p is: " << p << std::endl;
      // https://stackoverflow.com/a/596241/5843642
      // dst.data.at(p) = (r + r + r + b + g + g + g + g) >> 3;
      dst.data.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += 4;
    }
    for (int i = 0; i < dst.data.size(); i++) {
      std::cout << "value dst: " << (int)dst.data.at(i) << std::endl;
    }
  };

  Mat_t grayscale_t(u_char *src, int w, int h, int code) {
    Mat_t dst;
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
    std::cout << "cn3: " << cn3 << std::endl;

    // dst->resize(w, h, 1);
    std::cout << "dst size is: " << dst.data.size() << std::endl;
    // this should print a zero value
    std::cout << "value: " << (int)src[0] << std::endl;

    // code from jsartoolkit5
    int q = 0;
    int r;
    int g;
    int b;

    for (int p = 0; p < videosize; p++) {
      r = src[q + 0], g = src[q + 1], b = src[q + 2];
      std::cout << "p is: " << p << std::endl;
      // https://stackoverflow.com/a/596241/5843642
      // dst.data.at(p) = (r + r + r + b + g + g + g + g) >> 3;
      dst.data.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += 4;
    }
    for (int i = 0; i < dst.data.size(); i++) {
      std::cout << "value dst: " << (int)dst.data.at(i) << std::endl;
    }
    return dst;
  };
#ifdef __EMSCRIPTEN__
  _Mat_t grayscale_tt(u_char *src, int w, int h, int code) {
    _Mat_t dst;
    std::vector<u_char> output;
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
    std::cout << "cn3: " << cn3 << std::endl;

    // dst->resize(w, h, 1);
    std::cout << "value: " << (int)src[0] << std::endl;

    // code from jsartoolkit5
    int q = 0;
    u_char r;
    u_char g;
    u_char b;

    for (int p = 0; p < videosize; p++) {
      r = src[q + 0], g = src[q + 1], b = src[q + 2];
      std::cout << "p is: " << p << std::endl;
      // https://stackoverflow.com/a/596241/5843642
      output.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += 4;
    }
    std::cout << "output size: " << output.size() << std::endl;
    for(int i = 0; i < output.size(); i++) {
      std::cout << "output data value: " << (int)output.data()[i] << std::endl;
    }
    emscripten::val view{
        emscripten::typed_memory_view(output.size(), output.data())};
    auto result = emscripten::val::global("Uint8Array").new_(output.size());
    result.call<void>("set", view);
    dst.data = view;
    dst.rows = h;
    dst.cols = w;
    dst.channels = 1;
    dst.size = videosize;
    return dst;
  };

  _Mat_t grayscale_ttm(_Mat_t  src, int w, int h, int code) {
    _Mat_t dst;
    matrix_t output(w, h, 0x0100 | 0x01, 0x0100);
    std::vector<u_char> input = emscripten::convertJSArrayToNumberVector<u_char>(src.data);
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
    std::cout << "cn3: " << cn3 << std::endl;

    // dst->resize(w, h, 1);
    std::cout << "value: " << (int)input.at(0) << std::endl;

    // code from jsartoolkit5
    int q = 0;
    u_char r;
    u_char g;
    u_char b;

    for (int p = 0; p < videosize; p++) {
      r = input.at(q + 0), g = input.at(q + 1), b = input.at(q + 2);
      //std::cout << "p is: " << p << std::endl;
      // https://stackoverflow.com/a/596241/5843642
      output.dt->u8.push_back((r + r + r + b + g + g + g + g) >> 3);
      q += 4;
    }
    std::cout << "output size: " << output.size << std::endl;
    /*for(int i = 0; i < output.size; i++) {
      std::cout << "output data value: " << (int)output.dt->u8.data()[i] << std::endl;
    }*/
    emscripten::val view{
        emscripten::typed_memory_view(output.dt->u8.size(), output.dt->u8.data())};
    auto result = emscripten::val::global("Uint8Array").new_(output.dt->u8.size());
    result.call<void>("set", view);
    dst.data = view;
    dst.rows = h;
    dst.cols = w;
    dst.channels = 1;
    dst.size = videosize;
    return dst;
  };
#endif
};
} // namespace jsfeat

#endif