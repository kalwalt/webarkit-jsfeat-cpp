#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    register_vector<std::vector<u_char>>("vector_u_char");
    register_vector<KPoint_t>("vector_kpoint_t");

    class_<Matrix_t>("matrix_t")
    .constructor<int, int, int, emscripten::val>()
    .constructor<int, int, int>()
    .function("allocate", &Matrix_t::allocate)
    .function("resize", &Matrix_t::resize)
    .function("getPointer", &Matrix_t::getPointer)
    .property("cols", &Matrix_t::getCols, &Matrix_t::setCols)
    .property("rows", &Matrix_t::getRows, &Matrix_t::setRows)
    .property("type", &Matrix_t::getType, &Matrix_t::setType)
    .property("channel", &Matrix_t::getChannel, &Matrix_t::setChannel)
    .property("data", &Matrix_t::getData)
    .class_function("get", &Matrix_t::get);

    class_<Imgproc>("imgproc")
    .constructor<>()
    .function("grayscale", &Imgproc::grayscale, allow_raw_pointer<Matrix_t>())
    .function("grayscale_m", &Imgproc::grayscale_m, allow_raw_pointer<Matrix_t>(), allow_raw_pointer<Matrix_t>())
    .function("pyrdown", &Imgproc::pyrdown, allow_raw_pointer<Matrix_t>())
    .function("equalize_histogram", &Imgproc::equalize_histogram, allow_raw_pointer<Matrix_t>())
    .function("warp_affine", &Imgproc::warp_affine, allow_raw_pointer<Matrix_t>())
    .function("resample", select_overload<void(uintptr_t, uintptr_t, int, int)>(&Imgproc::resample));

    class_<Orb>("orb")
    .constructor<>()
    .function("describe", &Orb::describe, allow_raw_pointer<Matrix_t>(), allow_raw_pointer<Matrix_t>());

    class_<pyramid_t>("pyramid_t")
    .constructor<int>()
    .function("allocate", &pyramid_t::allocate)
    .function("build", &pyramid_t::build, allow_raw_pointer<Matrix_t>())
    .function("getPointer", &pyramid_t::getPointer)
    .function("getPointer_matrix", &pyramid_t::getPointer_matrix)
    .function("getMatrixData", &pyramid_t::getMatrixData)
    .property("levels", &pyramid_t::getLevels)
    .property("data", &pyramid_t::getData);

    class_<KeyPoint_t>("keypoint_t")
    .constructor<>()
    .constructor<int, int, int, int, float>()
    .function("getPointer", &KeyPoint_t::getPointer)
    .property("x", &KeyPoint_t::getX, &KeyPoint_t::setX)
    .property("y", &KeyPoint_t::getY, &KeyPoint_t::setY)
    .property("score", &KeyPoint_t::getScore, &KeyPoint_t::setScore)
    .property("level", &KeyPoint_t::getLevel, &KeyPoint_t::setLevel)
    .property("angle", &KeyPoint_t::getAngle, &KeyPoint_t::setAngle);

    class_<KeyPoints>("KeyPoints")
    .constructor<>()
    .constructor<size_t>()
    .property("corners", &KeyPoints::get_corners)
    .function("get_pointer", &KeyPoints::get_pointer);

    class_<Yape06>("yape06")
    .constructor<>()
    .constructor<int, int>()
    .function("detect", &Yape06::detect, allow_raw_pointer<Matrix_t>())
    .property("laplacian_threshold", &Yape06::get_laplacian_threshold, &Yape06::set_laplacian_threshold)
    .property("min_eigen_value_threshold", &Yape06::get_min_eigen_value_threshold, &Yape06::set_min_eigen_value_threshold);

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

    value_object<KPoint_t>("KPoint_t")
    .field("x", &KPoint_t::x)
    .field("y", &KPoint_t::y)
    .field("score", &KPoint_t::score)
    .field("level", &KPoint_t::level)
    .field("angle", &KPoint_t::angle);

};