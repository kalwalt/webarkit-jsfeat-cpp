//#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    class_<matrix_t>("matrix_t")
    .constructor<int, int, int, int>()
    .function("allocate", &matrix_t::allocate)
    .function("resize", &matrix_t::resize);
    function("toGrayscale", &toGrayscale, allow_raw_pointers());
    function("hello", &hello);
}