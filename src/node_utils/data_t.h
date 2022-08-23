#ifndef DATA_T_H
#define DATA_T_H

namespace jsfeat {
class data_t {
    public:
    int size;
    Array<u_char> u8;
    Array<int> i32;
    Array<float> f32;
    Array<double> f64;

    data_t(int size_in_bytes) {
        size = ((size_in_bytes + 7) | 0) & -8;
    }
};
} // namespace jsfeat

#endif