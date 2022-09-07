#include <imgproc/imgproc.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>

using namespace jsfeat;

void grayscale(u_char *src, int w, int h, matrix_t *dst, int code);

void grayscale_m(matrix_t *src, int w, int h, matrix_t *dst, int code);