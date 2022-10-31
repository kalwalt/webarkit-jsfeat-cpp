#ifndef KEYPOINT_T_H
#define KEYPOINT_T_H

class KeyPoint_t {
public:
  int x, y, score, level;
  float angle;
  KeyPoint_t() {
    this->x = 0;
    this->y = 0;
    this->score = 0;
    this->level = 0;
    this->angle = -1.0;
  }
  KeyPoint_t(int x, int y, int score, int level, float angle) {
    if (!x) {
      this->x = 0;
    }
    if (!y) {
      this->y = 0;
    }
    if (!score) {
      this->score = 0;
    }
    if (!level) {
      this->level = 0;
    }
    if (!angle) {
      this->angle = -1.0;
    }

    this->x = x;
    this->y = y;
    this->score = score;
    this->level = level;
    this->angle = angle;
  }
  auto set_x(int x) { this->x = x; }
  auto get_x() const { return this->x; }
  auto set_y(int y) { this->y = y; }
  auto get_y() const { return this->y; }
  auto set_score(int score) { this->score = score; }
  auto get_score() const { return this->score; }
  auto set_level(int level) { this->level = level; }
  auto get_level() const { return this->level; }
  auto set_angle(float angle) { this->angle = angle; }
  auto get_angle() const { return this->angle; }
  #ifdef __EMSCRIPTEN__
  auto get_pointer() { return reinterpret_cast<int>(this); }
  #endif
};

#endif