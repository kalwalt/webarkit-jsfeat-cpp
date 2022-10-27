#ifndef KEYPOINTS_H
#define KEYPOINTS_H

#include <types/types.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

namespace jsfeat {

class KeyPoints {

public:

  KeyPoints() {}

  KeyPoints(size_t size) {
    this->size = size;
    keypoint_t kpt(0, 0, 0, 0, -1);
    kpoints.assign(size, kpt);
  }

  KeyPoints(size_t size, keypoint_t kpt) {
    this->size = size;
    kpoints.assign(size, kpt);
  }

  auto get_key_points() const {
    return kpoints;
  }

  auto set_key_points(Array<keypoint_t> kpoints) {
    this->kpoints = kpoints;
  }

  #ifdef __EMSCRIPTEN__
  auto get_pointer() { return reinterpret_cast<int>(this); }

  emscripten::val get_corners() const {
    emscripten::val pointsArr = emscripten::val::array();
    KPoint_t pt;
    for(auto i = 0; i< kpoints.size(); i++){
      pt.x = kpoints[i].x;
      pt.y = kpoints[i].y;
      pt.level = kpoints[i].level;
      pt.score = kpoints[i].score;
      pt.angle = kpoints[i].angle;
      pointsArr.call<void>("push", pt);
    }
    return pointsArr;
  }
  #endif

  Array<keypoint_t> kpoints;

  private:
  size_t size;
};

} // namespace jsfeat

#endif