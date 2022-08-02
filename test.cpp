#include <matrix_t/matrix_t.h>
#include <orb/rectify_patch.h>
#include <imgproc/imgproc.h>
#include <stdlib.h>

using namespace jsfeat;

int main() {
  srand(1);
  for (int i = 0; i < 4; ++i) {
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    // cout << r << endl;
    rectify_patch(r, 2, 3, 4);
  }
  test_matrix_t();
  matrix_t t(3, 1, 0x0100 | 0x01, 0x0100);
  test2_matrix_t(t);
  matrix_t src(3, 3, 0x0100 | 0x04, 0x0100);
  matrix_t dst(3, 3, 0x0100 | 0x01, 0x0100);
  imgproc img;
  img.grayscale(src, 3, 3, dst, 0);

}