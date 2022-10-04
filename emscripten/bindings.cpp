#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    register_vector<std::vector<u_char>>("vector_u_char");

    class_<matrix_t>("matrix_t")
    .constructor<int, int, int, emscripten::val>()
    .constructor<int, int, int>()
    .function("allocate", &matrix_t::allocate)
    .function("resize", &matrix_t::resize)
    .function("getPointer", &matrix_t::getPointer)
    .property("cols", &matrix_t::getCols, &matrix_t::setCols)
    .property("rows", &matrix_t::getRows, &matrix_t::setRows)
    .property("type", &matrix_t::getType, &matrix_t::setType)
    .property("channel", &matrix_t::getChannel, &matrix_t::setChannel)
    .property("data", &matrix_t::getData)
    .class_function("get", &matrix_t::get);

    class_<imgproc>("imgproc")
    .constructor<>()
    .function("grayscale", &imgproc::grayscale, allow_raw_pointer<matrix_t>())
    .function("grayscale_m", &imgproc::grayscale_m, allow_raw_pointer<matrix_t>(),  allow_raw_pointer<matrix_t>())
    .function("pyrdown", &imgproc::pyrdown, allow_raw_pointer<matrix_t>())
    .function("warp_affine", &imgproc::warp_affine, allow_raw_pointer<matrix_t>())
    .function("resample", select_overload<void(uintptr_t, uintptr_t, int, int)>(&imgproc::resample));

    class_<orb>("orb")
    .constructor<>()
    .function("describe", &orb::describe, allow_raw_pointer<matrix_t>(), allow_raw_pointer<matrix_t>());

    class_<pyramid_t>("pyramid_t")
    .constructor<int>()
    .function("allocate", &pyramid_t::allocate)
    .function("build", &pyramid_t::build, allow_raw_pointer<matrix_t>())
    .function("getPointer", &pyramid_t::getPointer)
    .function("getPointer_matrix", &pyramid_t::getPointer_matrix)
    .function("getMatrixData", &pyramid_t::getMatrixData)
    .property("levels", &pyramid_t::getLevels)
    .property("data", &pyramid_t::getData);

    class_<keypoint_t>("keypoint_t")
    .constructor<int, int, int, int, float>()
    .function("getPointer", &keypoint_t::getPointer)
    .property("x", &keypoint_t::getX, &keypoint_t::setX)
    .property("y", &keypoint_t::getY, &keypoint_t::setY)
    .property("score", &keypoint_t::getScore, &keypoint_t::setScore)
    .property("level", &keypoint_t::getLevel, &keypoint_t::setLevel)
    .property("angle", &keypoint_t::getAngle, &keypoint_t::setAngle);

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
};