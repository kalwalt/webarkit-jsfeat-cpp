//#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    register_vector<std::vector<u_char>>("vector_u_char");

    class_<matrix_t>("matrix_t")
    .constructor<int, int, int, int>()
    .function("allocate", &matrix_t::allocate)
    .function("resize", &matrix_t::resize);

    enum_<Types>("Types")
    .value("U8_t", U8_t)
    .value("S32_t", S32_t)
    .value("F32_t", F32_t)
    .value("S64_t", S64_t)
    .value("F64_t", F64_t)
    .value("C1_t", C1_t)
    .value("C2_t", C2_t)
    .value("C3_t", C3_t)
    .value("C4_t", C4_t);

    enum_<Colors>("Colors")
    .value("COLOR_RGBA2GRAY", Colors::COLOR_RGBA2GRAY)
    .value("COLOR_RGB2GRAY", COLOR_RGB2GRAY)
    .value("COLOR_BGRA2GRAY", COLOR_BGRA2GRAY)
    .value("COLOR_BGR2GRAY", COLOR_BGR2GRAY);

    value_object<Mat_t>("Mat_t")
    .field("size", &Mat_t::size)
    .field("cols", &Mat_t::cols)
    .field("rows", &Mat_t::rows)
    .field("channels", &Mat_t::channels)
    .field("data", &Mat_t::data);

    value_object<_Mat_t>("_Mat_t")
    .field("size", &_Mat_t::size)
    .field("cols", &_Mat_t::cols)
    .field("rows", &_Mat_t::rows)
    .field("channels", &_Mat_t::channels)
    .field("data", &_Mat_t::data);


    function("toGrayscale", &toGrayscale, allow_raw_pointers());
    //function("Grayscale_t", &Grayscale_t, allow_raw_pointers());
    function("Grayscale_t", &Grayscale_t);
    function("Grayscale_tt", &Grayscale_tt);
}