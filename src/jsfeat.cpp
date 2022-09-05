#include <jsfeat.h>

using namespace jsfeat;


void grayscale(u_char *src, int w, int h, matrix_t *dst, int code) { 
    imgproc img;
    img.grayscale(src, w, h, dst, code);
};

void grayscale_m(matrix_t *src, int w, int h, matrix_t *dst, int code) { 
    imgproc img;
    img.grayscale_m(src, w, h, dst, code);
};