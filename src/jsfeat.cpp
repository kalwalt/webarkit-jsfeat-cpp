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