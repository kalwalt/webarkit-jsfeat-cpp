#include <imgproc/imgproc.h>
#include <matrix_t/matrix_t.h>
#include <types/types.h>

using namespace jsfeat;

void grayscale(u_char *src, int w, int h, matrix_t *dst, int code);

void grayscale_s(u_char *src, int w, int h, Mat_t dst, int code);

Mat_t grayscale_t(u_char *src, int w, int h, int code);
