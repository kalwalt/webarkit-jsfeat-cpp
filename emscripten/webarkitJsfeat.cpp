#include <AR/ar.h>
#include <AR2/config.h>
#include <AR2/imageFormat.h>
#include <AR2/util.h>
#include <WebARKit/WebARKitLog.h>
#include <emscripten.h>
#include <jsfeat.h>
#include <stdio.h>

#include <string>
#include <vector>

using namespace jsfeat;

extern "C" {

emscripten::val load_jpeg(const char* filename) {
  char* ext;
  char buf1[512], buf2[512];

  AR2JpegImageT* jpegImage;

  if (!filename) return emscripten::val::null();
  ext = arUtilGetFileExtensionFromPath(filename, 1);
  if (!ext) {
    webarkitLOGe(
        "Error: unable to determine extension of file '%s'. Exiting.\n",
        filename);
  }
  if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0 ||
      strcmp(ext, "jpe") == 0) {
    webarkitLOGi("Waiting for the jpeg...");
    webarkitLOGi("Reading JPEG file...");
    ar2UtilDivideExt(filename, buf1, buf2);
    jpegImage = ar2ReadJpegImage(buf1, buf2);
    if (jpegImage == NULL) {
      webarkitLOGe(
          "Error: unable to read JPEG image from file '%s'. Exiting.\n",
          filename);
    }
    webarkitLOGi("   Done.");

    if (jpegImage->nc != 1 && jpegImage->nc != 3) {
      ARLOGe(
          "Error: Input JPEG image is in neither RGB nor grayscale format. "
          "%d bytes/pixel %sformat is unsupported. Exiting.\n",
          jpegImage->nc, (jpegImage->nc == 4 ? "(possibly CMYK) " : ""));
    }
    webarkitLOGi("JPEG image number of channels: '%d'", jpegImage->nc);
    webarkitLOGi("JPEG image width is: '%d'", jpegImage->xsize);
    webarkitLOGi("JPEG image height is: '%d'", jpegImage->ysize);
    webarkitLOGi("JPEG image, dpi is: '%d'", jpegImage->dpi);

    if (jpegImage->dpi == 0.0f) {
      webarkitLOGw(
          "JPEG image '%s' does not contain embedded resolution data, and no "
          "resolution specified on command-line.",
          filename);
    }

  } else if (strcmp(ext, "png") == 0) {
    webarkitLOGe(
        "Error: file has extension '%s', which is not supported for "
        "reading. Exiting.\n",
        ext);
    free(ext);
  }
  auto jpegSize = jpegImage->xsize * jpegImage->ysize * jpegImage->nc;
  emscripten::val obj = emscripten::val::object();
  emscripten::val view{
      emscripten::typed_memory_view(jpegSize, jpegImage->image)};
  auto result = emscripten::val::global("Uint8Array").new_(jpegSize);
  result.call<void>("set", view);
  obj.set("width", jpegImage->xsize);
  obj.set("height", jpegImage->ysize);
  obj.set("dpi", jpegImage->dpi);
  obj.set("data", result);

  free(ext);
  free(jpegImage);

  return obj;
};

emscripten::val load_jpeg_data(std::string filename) {
  auto out = load_jpeg(filename.c_str());
  return out;
};

emscripten::val yape06_detect(emscripten::val inputSrc, int w, int h) {
  auto src = emscripten::convertJSArrayToNumberVector<u_char>(inputSrc);
  Imgproc imgproc;
  Yape06 yape;

  std::unique_ptr<KeyPoints> keypoints = std::make_unique<KeyPoints>(w * h);
  std::unique_ptr<Matrix_t> lev0_img = std::make_unique<Matrix_t>(w, h, ComboTypes::U8C1_t);

  imgproc.grayscale_internal<u_char, Matrix_t>(src.data(), w, h, lev0_img.get(), Colors::COLOR_RGBA2GRAY);
  imgproc.gaussian_blur_internal(lev0_img.get(), lev0_img.get(), 5, 2);
  auto obj = yape.detect_internal(lev0_img.get(), keypoints.get(), 17);

  emscripten::val outObj = emscripten::val::object();
  emscripten::val pointsArr = emscripten::val::array();
  KPoint_t pt;
  for (auto i = 0; i < obj.pts.kpoints.size(); i++) {
    pt.x = obj.pts.kpoints[i].x;
    pt.y = obj.pts.kpoints[i].y;
    pt.level = obj.pts.kpoints[i].level;
    pt.score = obj.pts.kpoints[i].score;
    pt.angle = obj.pts.kpoints[i].angle;
    pointsArr.call<void>("push", pt);
  }
  outObj.set("count", obj.count);
  outObj.set("points", pointsArr);

  return outObj;
};

}

#include "bindings.cpp"