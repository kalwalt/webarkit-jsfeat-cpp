#include <jsfeat.h>

using namespace jsfeat;


void grayscale(u_char *src, int w, int h, matrix_t *dst, int code) { 
    imgproc img;
    img.grayscale(src, w, h, dst, code);
};

void grayscale_s(u_char *src, int w, int h, Mat_t dst, int code) { 
    imgproc img;
    img.grayscale_s(src, w, h, dst, code);
};

Mat_t grayscale_t(u_char *src, int w, int h, int code) { 
    imgproc img;
    return img.grayscale_t(src, w, h, code);
};

_Mat_t grayscale_tt(u_char *src, int w, int h, int code) { 
    imgproc img;
    return img.grayscale_tt(src, w, h, code);
};

_Mat_t grayscale_ttm(_Mat_t src, int w, int h, int code) { 
    imgproc img;
    return img.grayscale_ttm(src, w, h, code);
};

_Mat_t grayscale_jsfeat(_Mat_t src, int w, int h, int code) { 
    imgproc img;
    return img.grayscale_jsfeat(src, w, h, code);
};