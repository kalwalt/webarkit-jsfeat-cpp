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
  ~data_t() {
#ifdef DEBUG_EM
    JSLOGd("deleting data_t");
#endif
  }
};
} // namespace jsfeat

#endif