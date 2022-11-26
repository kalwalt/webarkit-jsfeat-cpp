#include <AR/ar.h>
#include <AR2/config.h>
#include <AR2/imageFormat.h>
#include <AR2/util.h>
#include <WebARKit/WebARKitLog.h>
#include <emscripten.h>
#include <jsfeat.h>
#include <stdio.h>

#include <cmath>
#include <memory>
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

void train_orb_pattern_internal(const char* filename) {
  char* ext;
  char buf1[512], buf2[512];

  AR2JpegImageT* jpegImage;

  auto lev = 0, i = 0;
  auto sc = 1.0;
  auto max_pattern_size = 512;
  auto max_per_level = 300;
  auto sc_inc = std::sqrt(2.0);  // magic number ;)
  auto new_width = 0, new_height = 0;
  // var lev_corners, lev_descr;
  auto corners_num = 0;

  // if (!filename) return emscripten::val::null();
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
  webarkitLOGi("Image done!");

  JSLOGi("Starting detection routine...\n");

  Orb orb;
  Imgproc imgproc;
  detectors::Detectors detectors;
  std::unique_ptr<Matrix_t> lev0_img = std::make_unique<Matrix_t>(jpegImage->xsize, jpegImage->ysize, ComboTypes::U8C1_t);
  std::unique_ptr<Matrix_t> lev_img = std::make_unique<Matrix_t>(jpegImage->xsize, jpegImage->ysize, ComboTypes::U8C1_t);
  Array<std::unique_ptr<Matrix_t>> pattern_corners;
  std::cout << "after orb" << std::endl;

  auto sc0 = std::min(max_pattern_size / jpegImage->ysize, max_pattern_size / jpegImage->xsize);
  new_width = (jpegImage->ysize * sc0) | 0;
  new_height = (jpegImage->xsize * sc0) | 0;
  auto num_train_levels = 4;

  auto i_u8_size = jpegImage->xsize * jpegImage->ysize * jpegImage->nc;
  Array<u_char> i_u8(jpegImage->image, jpegImage->image + i_u8_size);
  std::unique_ptr<Matrix_t> img_u8 = std::make_unique<Matrix_t>(jpegImage->xsize, jpegImage->ysize, ComboTypes::U8C4_t, i_u8);
  
  JSLOGi("Resampling image...");
  
  //imgproc.resample(img_u8.get(), lev0_img.get(), new_width, new_height);
  
  JSLOGi("Image resampled, starting pyramid now...");
  // prepare preview
  std::unique_ptr<Matrix_t> pattern_preview = std::make_unique<Matrix_t>(jpegImage->xsize >> 1, jpegImage->ysize >> 1, ComboTypes::U8C1_t);
  imgproc.pyrdown_internal(lev0_img.get(), pattern_preview.get());

  Array<KeyPoints*> lev_corners(4);
  Array<std::unique_ptr<Matrix_t>> pattern_descriptors;

  for (lev = 0; lev < num_train_levels; ++lev) {
    // what we should do with this code?
    // pattern_corners[lev] = [];
    // lev_corners = pattern_corners[lev];

    // preallocate corners array
    i = (new_width * new_height) >> lev;
    while (--i >= 0) {
      lev_corners[lev]->set_size(i);
      lev_corners[lev]->allocate();
    }
    std::cout << "Num. of level: " << lev << std::endl;
    pattern_descriptors.push_back(std::unique_ptr<Matrix_t>(new Matrix_t(32, max_per_level, ComboTypes::U8C1_t)));
  }

  //std::cout << "Size of first lev_corners: " << lev_corners[0]->kpoints.size() << std::endl;

  imgproc.gaussian_blur_internal(lev0_img.get(), lev_img.get(), 5, 2);  // this is more robust

  JSLOGi("After Gaussian blur");

  corners_num = detectors.detect_keypoints(lev_img.get(), lev_corners[0], max_per_level);

  // orb.describe(lev_img.get(), lev_corners[0], corners_num, lev_descr.get());
  // This probablly will work in a near future
  // orb.describe(lev_img.get(), lev_corners[0], corners_num, &pattern_descriptors[0]);

  // console.log("train " + lev_img.cols + "x" + lev_img.rows + " points: " + corners_num);
  std::cout << "Corners num: " << (int)corners_num << std::endl;
  //JSLOGi("Corners num:  %i", corners_num);
  //JSLOGi("train %i x %i points: %i\n", lev_img.get()->get_cols(), lev_img.get()->get_rows(), corners_num);
  //std::cout << "train " << lev_img.get()->get_cols() << " x " << lev_img.get()->get_rows() << " points: " << corners_num << std::endl;
  free(ext);
  free(jpegImage);
};

void train_orb_pattern(std::string filename) {
  train_orb_pattern_internal(filename.c_str());
}
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