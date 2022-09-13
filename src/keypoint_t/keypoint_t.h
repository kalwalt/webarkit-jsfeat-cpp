#ifndef KEYPOINT_T_H
#define KEYPOINT_T_H

class keypoint_t {
public:
  int x, y, score, level;
  float angle;
  keypoint_t(int x, int y, int score, int level, float angle) {
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
  auto setX(int x) { this->x = x; }
  auto getX() const { return this->x; }
  auto setY(int y) { this->y = y; }
  auto getY() const { return this->y; }
  auto setScore(int score) { this->score = score; }
  auto getScore() const { return this->score; }
  auto setLevel(int level) { this->level = level; }
  auto getLevel() const { return this->level; }
  auto setAngle(float angle) { this->angle = angle; }
  auto getAngle() const { return this->angle; }
};

#endif