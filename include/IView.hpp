#pragma once
#include <termios.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <utility>

struct loc {
  int first, second;

  int dist(const loc& c) const {
    return std::abs(c.first - first) + std::abs(c.second - second);
  }

  bool operator==(const loc& rhs) const {
    return first == rhs.first && second == rhs.second;
  }
};

struct Snake {
 public:
  int length;
  int last_length = 0;
  int direction = 1;
  bool is_controlled = false;
  bool is_bot = false;
  loc head;
  loc tail;
  std::list<loc> body;
};

struct Rabbit {
 private:
 public:
  loc xy;
};

class IView {
 protected:
  struct termios old_term;

 public:
  bool is_game_goes = true;
  loc window_dimensions = {0, 0};
  std::list<std::function<void(int)>> onkeys;
  std::list<std::function<void()>> ontimes;
  std::function<void()> draw_without_update;

  static IView* view;

  void set_onkey(std::function<void(int)> f);
  void set_ontimes(std::function<void()> f);
  void set_draw_without_update(std::function<void()> f);
  IView();

  virtual void print_game_name(std::string game_name) = 0;
  virtual void render() = 0;
  virtual void render(std::list<Rabbit>& rabbits, std::list<Snake>& snakes) = 0;
  virtual void bye_print() = 0;
  virtual void draw_rabbit(const Rabbit& rabbit) = 0;
  virtual void draw_snake(const Snake& snake) = 0;
  virtual void mainloop() = 0;
  virtual ~IView();

  static IView* get(std::string s = "text");
};