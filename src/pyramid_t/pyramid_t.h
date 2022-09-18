#ifndef PYRAMID_T_H
#define PYRAMID_T_H

#include <emscripten/val.h>
#include <imgproc/imgproc.h>
#include <matrix_t/matrix_t.h>
#include <vector>

namespace jsfeat {

class pyramid_t {
public:
  int levels;
  std::vector<matrix_t *> mt;
  emscripten::val data = emscripten::val::array();

  pyramid_t(int levels) {
    this->levels = levels;
    // this->data (levels, 0);
  }
  void allocate(int start_w, int start_h, int data_type) {
    int i = this->levels;
    while (--i >= 0) {
        this->mt[i] = new matrix_t(start_w >> i, start_h >> i, data_type, Uint8Array);
        this->mt[i]->allocate();
    }
  }
  void build(uintptr_t inputSrc, bool skip_first_level) {
    auto input = reinterpret_cast<matrix_t *>(inputSrc);
    if (!skip_first_level) {
      skip_first_level = true;
    }
    // just copy data to first level
    int i = 2;
    matrix_t *a = input;
    matrix_t *b = this->mt[0];
    if (!skip_first_level) {
      int j = input->cols * input->rows;
      while (--j >= 0) {
        b->dt->u8[j] = input->dt->u8[j];
      }
    }
    b = this->mt[1];
    this->proc.pyrdown_internal(a, b);
    for (; i < this->levels; ++i) {
      a = b;
      b = this->mt[i];
      this->proc.pyrdown_internal(a, b);
    }
  }

  auto getPointer() { return reinterpret_cast<int>(this); }

  auto getPointer_matrix(int index) { return reinterpret_cast<int>(this->mt[index]); }

  int getLevels() const {
    return this->levels;
  }

  emscripten::val getData() const {
    emscripten::val data = emscripten::val::array();
 
    for (int i = 0; i < this->levels; i++){
        emscripten::val view{
            emscripten::typed_memory_view(mt[i]->dt->u8.size(), mt[i]->dt->u8.data())};
        auto result = emscripten::val::global("Uint8Array").new_(mt[i]->dt->u8.size());
        result.call<void>("set", view);
        data.call<void>("push", result);
    }
    return data;
  }

  private:
  imgproc proc;
};



} // namespace jsfeat

#endif