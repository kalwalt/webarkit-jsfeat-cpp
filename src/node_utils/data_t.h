#ifndef DATA_T_H
#define DATA_T_H

#include <types/types.h>

namespace jsfeat {
class data_t {
    public:
    Array<u_char> u8;
    Array<int> i32;
    Array<float> f32;
    Array<double> f64;
    data_t() {}
};
} // namespace jsfeat

#endif