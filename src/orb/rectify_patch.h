#ifndef RECTIFY_PATCH_H
#define RECTIFY_PATCH_H

#include <array>
#include <cmath>
#include <imgproc/warp_affine.h>
#include <iostream>
#include <matrix_t/matrix_t.h>

using namespace std;
namespace jsfeat {
typedef std::array<unsigned char, 9> Matrix3x3;
template <std::size_t S> using Array_uchar = std::array<unsigned char, S>;

void rectify_patch(float angle, int px, int py, int psize) {
  Array_uchar<9> H;
  float cosine = std::cos(angle);
  float sine = std::sin(angle);
  H.at(0) = (unsigned char)cosine;
  H.at(1) = (unsigned char)-sine;
  H.at(2) = (unsigned char)((-cosine + sine) * psize * 0.5 + px);
  H.at(3) = (unsigned char)sine;
  H.at(4) = (unsigned char)cosine;
  H.at(5) = (unsigned char)((-sine - cosine) * psize * 0.5 + py);
  for (int i = 0; i < 9; i++) {
    cout << i << ' ' << H.at(i);
    cout << '\n';
  }
  warp_affine(0, 0, 0, 0);
};

void test_matrix_t() {
  jsfeat::matrix_t m(2, 2, 0x0100, 0x0100);
  // cout << m << endl;
  m.allocate();
}
void test2_matrix_t(matrix_t t) {
  printf("second matrix_t test\n");
  t.allocate();
}
} // namespace jsfeat

#endif