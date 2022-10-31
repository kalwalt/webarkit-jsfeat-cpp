#include <types/types.h>

namespace jsfeat {
template <typename A, typename B>
auto compute_laplacian(Array<A>& src, Array<B>& dst, int w, int h, int Dxx,
                       int Dyy, int sx, int sy, int ex, int ey) {
  auto y = 0, x = 0, yrow = (sy * w + sx) | 0, row = yrow;

  for (y = sy; y < ey; ++y, yrow += w, row = yrow) {
    for (x = sx; x < ex; ++x, ++row) {
      dst[row] = -4 * src[row] + src[row + Dxx] + src[row - Dxx] +
                 src[row + Dyy] + src[row - Dyy];
    }
  }

}
template <typename A, typename B>
auto compute_laplacian(Array<A> src, int w, int h, int Dxx,
                       int Dyy, int sx, int sy, int ex, int ey) {
  auto y = 0, x = 0, yrow = (sy * w + sx) | 0, row = yrow;
  Array<B> dst(src.size(), 0);

  for (y = sy; y < ey; ++y, yrow += w, row = yrow) {
    for (x = sx; x < ex; ++x, ++row) {
      dst[row] = -4 * src[row] + src[row + Dxx] + src[row - Dxx] +
                 src[row + Dyy] + src[row - Dyy];
    }
  }
  return dst;
}
template <typename A>
auto hessian_min_eigen_value(Array<A>& src, int off, int tr, int Dxx, int Dyy,
                             int Dxy, int Dyx) {
  auto Ixx = -2 * src[off] + src[off + Dxx] + src[off - Dxx];
  auto Iyy = -2 * src[off] + src[off + Dyy] + src[off - Dyy];
  auto Ixy = src[off + Dxy] + src[off - Dxy] - src[off + Dyx] - src[off - Dyx];
  // auto sqrt_delta =
  //     (std::sqrt(((Ixx - Iyy) * (Ixx - Iyy) + 4 * Ixy * Ixy))) | 0;

  auto sqrt_delta = (std::sqrt(((Ixx - Iyy) * (Ixx - Iyy) + 4 * Ixy * Ixy)));

  return std::min(std::abs(tr - sqrt_delta), std::abs(-(tr + sqrt_delta)));
}
} // namespace jsfeat
