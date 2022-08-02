#ifndef IMGPROC_H
#define IMGPROC_H

#include <matrix_t/matrix_t.h>
#include <types/types.h>

namespace jsfeat {
class imgproc {
public:
  int code;
  void grayscale(matrix_t src, int w, int h, matrix_t dst, int code) {
    // this is default image data representation in browser
    if (!code) {
      code = Colors::COLOR_RGBA2GRAY;
    }
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

    dst.resize(w, h, 1);
    u_char *dst_u8 = dst.data;

    for (y = 0; y < h; ++y, j += w, i += w * cn) {
      for (x = 0, ir = i, jr = j; x <= w - 4; x += 4, ir += cn << 2, jr += 4) {
        dst_u8[jr] = (src.data[ir] * coeff_r + src.data[ir + 1] * coeff_g +
                      src.data[ir + 2] * coeff_b + 8192) >>
                     14;
        dst_u8[jr + 1] =
            (src.data[ir + cn] * coeff_r + src.data[ir + cn + 1] * coeff_g +
             src.data[ir + cn + 2] * coeff_b + 8192) >>
            14;
        dst_u8[jr + 2] =
            (src.data[ir + cn2] * coeff_r + src.data[ir + cn2 + 1] * coeff_g +
             src.data[ir + cn2 + 2] * coeff_b + 8192) >>
            14;
        dst_u8[jr + 3] =
            (src.data[ir + cn3] * coeff_r + src.data[ir + cn3 + 1] * coeff_g +
             src.data[ir + cn3 + 2] * coeff_b + 8192) >>
            14;
      }
      for (; x < w; ++x, ++jr, ir += cn) {
        dst_u8[jr] = (src.data[ir] * coeff_r + src.data[ir + 1] * coeff_g +
                      src.data[ir + 2] * coeff_b + 8192) >>
                     14;
      }
    }
  };
};
} // namespace jsfeat

#endif