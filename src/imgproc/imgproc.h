#ifndef IMGPROC_H
#define IMGPROC_H

#include <iostream>
#include <matrix_t/matrix_t.h>
#include <types/types.h>
#include <vector>

namespace jsfeat {

class imgproc {
public:
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

    dst->resize(w, h, 1);

    if (src->dt->u8.empty()) {
      std::cout << "vector is empty" << std::endl;
    }

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      // probably we can do this in javascript but not in C++
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        dst->dt->u8.at(jr) =
            (src->dt->u8.at(ir) * coeff_r + src->dt->u8.at(ir + 1) * coeff_g +
             src->dt->u8.at(ir + 2) * coeff_b + 8192) >>
            14;
        dst->dt->u8.at(jr + 1) =
            (src->dt->u8.at(ir + cn) * coeff_r +
             src->dt->u8.at(ir + cn + 1) * coeff_g +
             src->dt->u8.at(ir + cn + 2) * coeff_b + 8192) >>
            14;
        dst->dt->u8.at(jr + 2) =
            (src->dt->u8.at(ir + cn2) * coeff_r +
             src->dt->u8.at(ir + cn2 + 1) * coeff_g +
             src->dt->u8.at(ir + cn2 + 2) * coeff_b + 8192) >>
            14;
        dst->dt->u8.at(jr + 3) =
            (src->dt->u8.at(ir + cn3) * coeff_r +
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
    // Only for testing. It will be deleted in the future.
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

    dst->resize(w, h, 1);

    // code from jsartoolkit5
    int q = 0;
    /*unsigned char r;
    unsigned char g;
    unsigned char b;*/

    int r;
    int g;
    int b;

    for (int p = 0; p < videosize; p++) {
      r = src[q + 0];
      g = src[q + 1];
      b = src[q + 2];

      // std::cout << "p is: " << p << std::endl;
      //  https://stackoverflow.com/a/596241/5843642
      dst->dt->u8.at(p) = (r + r + r + b + g + g + g + g) >> 3;

      q += 4;
    }
    std::cout << "Grayscale works!" << std::endl;
    /*for (int i = 0; i < dst->dt->u8.size(); i++) {
      std::cout << "value dst: " << (int)dst->dt->u8.at(i) << std::endl;
    }*/
  };
};
} // namespace jsfeat

#endif