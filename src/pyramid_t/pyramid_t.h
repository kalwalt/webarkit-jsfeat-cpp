#ifndef PYRAMID_T_H
#define PYRAMID_T_H

#include <emscripten/val.h>
#include <imgproc/imgproc.h>
#include <matrix_t/matrix_t.h>
#include <vector>

namespace jsfeat {

class Pyramid_t {
public:
  int levels;
  std::vector<Matrix_t *> mt;
  emscripten::val data = emscripten::val::array();

  Pyramid_t(int levels) {
    this->levels = levels;
    // this->data (levels, 0);
  }
  void allocate(int start_w, int start_h, int data_type) {
    int i = this->levels;
    while (--i >= 0) {
        this->mt[i] = new Matrix_t(start_w >> i, start_h >> i, data_type, Uint8Array);
        this->mt[i]->allocate();
    }
  }
  void build(uintptr_t inputSrc, bool skip_first_level) {
    auto input = reinterpret_cast<Matrix_t *>(inputSrc);
    if (!skip_first_level) {
      skip_first_level = true;
    }
    // just copy data to first level
    int i = 2;
    Matrix_t *a = input;
    Matrix_t *b = this->mt[0];
    if (!skip_first_level) {
      int j = input->cols * input->rows;
      while (--j >= 0) {
        b->u8[j] = input->u8[j];
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

  auto get_pointer() { return reinterpret_cast<int>(this); }

  auto get_pointer_matrix(int index) { return reinterpret_cast<int>(this->mt[index]); }

  int get_levels() const {
    return this->levels;
  }

  emscripten::val get_matrix_data() const { 
    emscripten::val obj = emscripten::val::object();
    emscripten::val cols = emscripten::val::array();
    emscripten::val rows = emscripten::val::array();
    for (int i = 0; i < this->levels; i++){
      cols.call<void>("push", mt[i]->cols);
      rows.call<void>("push", mt[i]->rows);
    }
    obj.set("cols", cols);
    obj.set("rows", rows);
    return obj;
    }

  emscripten::val get_data() const {
    emscripten::val data = emscripten::val::array();
 
    for (int i = 0; i < this->levels; i++){
        emscripten::val view{
            emscripten::typed_memory_view(mt[i]->u8.size(), mt[i]->u8.data())};
        auto result = emscripten::val::global("Uint8Array").new_(mt[i]->u8.size());
        result.call<void>("set", view);
        data.call<void>("push", result);
    }
    return data;
  }

  private:
  Imgproc proc;
};



} // namespace jsfeat

#endif