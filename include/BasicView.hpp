#pragma once
#include "Game.hpp"
#include "IView.hpp"

class BasicView : public IView {
 private:
  void clear_screen();
  void set_color(int brightness, int color);
  void move_cursor(int x, int y);
  void draw_border(char symbol, int start_x, int start_y, int end_x, int end_y);
  void draw_rabbit(const Rabbit &rabbit);
  void draw_snake(const Snake &snake);

 public:
  BasicView();
  ~BasicView();
  virtual void print_game_name(std::string game_name);
  virtual void render();
  virtual void render(std::list<Rabbit> &rabbits, std::list<Snake> &snakes);
  virtual void bye_print();
  virtual void mainloop();
};
