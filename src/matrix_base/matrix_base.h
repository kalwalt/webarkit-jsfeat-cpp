#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

namespace jsfeat {
class MatrixBase {
 public:
  MatrixBase(){};
  virtual ~MatrixBase(){};
  virtual void allocate() = 0;
  virtual void resize(int c, int r, int ch) = 0;
};
};  // namespace jsfeat
#endif