#include <emscripten/bind.h>

using namespace emscripten;


EMSCRIPTEN_BINDINGS(webarkit) {
    register_vector<std::vector<u_char>>("vector_u_char");
    register_vector<KPoint_t>("vector_kpoint_t");

    class_<Matrix_t>("matrix_t")
    .constructor<>()
    .constructor<Matrix_t>()
    .constructor<int, int, int, emscripten::val>()
    .constructor<int, int, int>()
    .function("allocate", &Matrix_t::allocate)
    .function("resize", &Matrix_t::resize)
    .function("getPointer", &Matrix_t::get_pointer)
    .property("cols", &Matrix_t::get_cols, &Matrix_t::set_cols)
    .property("rows", &Matrix_t::get_rows, &Matrix_t::set_rows)
    .property("type", &Matrix_t::get_type, &Matrix_t::set_type)
    .property("channel", &Matrix_t::get_channel_m, &Matrix_t::set_channel_m)
    .property("data", &Matrix_t::get_data)
    .class_function("get", &Matrix_t::get);

    class_<Matrix_smart>("matrix_smart")
    .constructor<int, int, int>()
    .smart_ptr<std::shared_ptr<Matrix_smart>>("matrix_smart")
    //.smart_ptr_constructor<int, int, int>("Matrix_smart", &std::make_shared<Matrix_smart>)
    .function("allocate", &Matrix_smart::allocate)
    .function("resize", &Matrix_t::resize)
    .function("getSmartPointer", &Matrix_smart::get_smart_pointer)
    .property("cols", &Matrix_smart::get_cols, &Matrix_smart::set_cols)
    .property("rows", &Matrix_smart::get_rows, &Matrix_smart::set_rows)
    .property("type", &Matrix_smart::get_type, &Matrix_smart::set_type)
    .property("channel", &Matrix_smart::get_channel_m, &Matrix_smart::set_channel_m)
    .property("data", &Matrix_smart::get_data)
    .property("data", &Matrix_smart::get_data);

    class_<Imgproc>("imgproc")
    .constructor<>()
    .function("gaussian_blur", &Imgproc::gaussian_blur, allow_raw_pointer<Matrix_t>(), allow_raw_pointer<Matrix_t>())
    .function("grayscale", select_overload<void(emscripten::val, int, int, Matrix_t*, int)>(&Imgproc::grayscale_rgba), allow_raw_pointer<Matrix_t>())
    .function("grayscale_m", &Imgproc::grayscale_m, allow_raw_pointer<Matrix_t>(), allow_raw_pointer<Matrix_t>())
    .function("grayscale_rgba", select_overload<void(emscripten::val, int, int, std::shared_ptr<Matrix_smart>, int)>(&Imgproc::grayscale_rgba))
    .function("grayscale_rgba_t", select_overload<void(emscripten::val, int, int, std::shared_ptr<Matrix_t>, int)>(&Imgproc::grayscale_rgba))
    .function("grayscale_rgba_standard", select_overload<void(emscripten::val, int, int, Matrix_t*, int)>(&Imgproc::grayscale_rgba), allow_raw_pointer<Matrix_t>())
    .function("pyrdown", &Imgproc::pyrdown, allow_raw_pointer<Matrix_t>())
    .function("equalize_histogram", &Imgproc::equalize_histogram, allow_raw_pointer<Matrix_t>())
    .function("warp_affine", &Imgproc::warp_affine, allow_raw_pointer<Matrix_t>())
    .function("resample", select_overload<void(uintptr_t, uintptr_t, int, int)>(&Imgproc::resample));

    class_<Orb>("orb")
    .constructor<>()
    .function("describe", &Orb::describe, allow_raw_pointer<Matrix_t>(), allow_raw_pointer<Matrix_t>());

    class_<Pyramid_t>("pyramid_t")
    .constructor<int>()
    .function("allocate", &Pyramid_t::allocate)
    .function("build", &Pyramid_t::build, allow_raw_pointer<Matrix_t>())
    .function("getPointer", &Pyramid_t::get_pointer)
    .function("getPointer_matrix", &Pyramid_t::get_pointer_matrix)
    .function("getMatrixData", &Pyramid_t::get_matrix_data)
    .property("levels", &Pyramid_t::get_levels)
    .property("data", &Pyramid_t::get_data);

    class_<KeyPoint_t>("keypoint_t")
    .constructor<>()
    .constructor<int, int, int, int, float>()
    .function("getPointer", &KeyPoint_t::get_pointer)
    .property("x", &KeyPoint_t::get_x, &KeyPoint_t::set_x)
    .property("y", &KeyPoint_t::get_y, &KeyPoint_t::set_y)
    .property("score", &KeyPoint_t::get_score, &KeyPoint_t::set_score)
    .property("level", &KeyPoint_t::get_level, &KeyPoint_t::set_level)
    .property("angle", &KeyPoint_t::get_angle, &KeyPoint_t::set_angle);

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

    enum_<ComboTypes>("ComboTypes")
    .value("U8C1_t", U8C1_t)
    .value("U8C3_t", U8C3_t)
    .value("U8C4_t", U8C4_t)
    .value("F32C1_t", F32C1_t)
    .value("F32C2_t", F32C2_t)
    .value("S32C1_t", S32C1_t)
    .value("S32C2_t", S32C2_t);

    enum_<ColorsSpace>("ColorsSpace")
    .value("RGBA", RGBA)
    .value("RGB", RGB)
    .value("GRAY", GRAY);

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

    // Extern jsfeat functions

    function("load_jpeg_data", &load_jpeg_data);

};