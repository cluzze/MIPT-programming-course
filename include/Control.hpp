#pragma once

#include <iostream>

#include "Game.hpp"

class Control {
 private:
  Game &game;
  Snake &snake;

 public:
  Control(Game &game_, Snake &snake_);
  void get_user_input();
  void key_pressed(int key);
  void timer();
  void draw_without_update();

  void silly_bot();
  void intelligent_bot();
  loc next_generation(std::vector<loc> &generation);
  bool in_bounds(int len1, int len2);
};

class Cell {
 public:
  loc cell;

  Cell *left = nullptr;
  Cell *right = nullptr;
  Cell *up = nullptr;
  Cell *down = nullptr;

  Cell(loc cell_) { cell = cell_; };
};
