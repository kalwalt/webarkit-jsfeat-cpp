#ifndef DATA_T_H
#define DATA_T_H

#include <jslog/jslog.h>
#include <types/types.h>

namespace jsfeat {
class data_t {
public:
  Array<u_char> u8;
  Array<int> i32;
  Array<float> f32;
  Array<double> f64;
  data_t() {}
  data_t(std::size_t size, int data_type) {
    if (data_type == Types::U8_t) {
      u8.assign(size, 0);
    } else if (data_type == Types::S32_t) {
      i32.assign(size, 0);
    } else if (data_type == Types::F32_t) {
      f32.assign(size, 0);
    } else if (data_type == Types::F64_t) {
      f64.assign(size, 0);
    }
  }
  ~data_t() {
#ifdef DEBUG_EM
    JSLOGd("deleting data_t");
#endif
  }
};
} // namespace jsfeat

#endif