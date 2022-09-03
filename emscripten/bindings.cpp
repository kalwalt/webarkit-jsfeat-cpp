//#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    register_vector<std::vector<u_char>>("vector_u_char");

    class_<matrix_t>("matrix_t")
    .constructor<int, int, int, emscripten::val>()
    .function("allocate", &matrix_t::allocate)
    .function("resize", &matrix_t::resize)
    .function("getPointer", &matrix_t::getPointer)
    .property("cols", &matrix_t::getCols, &matrix_t::setCols)
    .property("rows", &matrix_t::getRows, &matrix_t::setRows)
    .property("type", &matrix_t::getType, &matrix_t::setType)
    .property("channel", &matrix_t::getChannel, &matrix_t::setChannel)
    .property("data", &matrix_t::getData)
    .class_function("get", &matrix_t::get);

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

    value_object<_Mat_t>("_Mat_t")
    .field("size", &_Mat_t::size)
    .field("cols", &_Mat_t::cols)
    .field("rows", &_Mat_t::rows)
    .field("channels", &_Mat_t::channels)
    .field("data", &_Mat_t::data);

    function("Grayscale_tt", &Grayscale_tt);
    function("Grayscale_ttm", &Grayscale_ttm);
    function("Grayscale_jsfeat", &Grayscale_jsfeat);
}