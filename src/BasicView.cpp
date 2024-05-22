#include "BasicView.hpp"

#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>

const int BUFSIZE = 10;

void BasicView::clear_screen() { std::cout << "\e[H\e[J"; }

void BasicView::render() {
  clear_screen();
  set_color(1, 35);

  struct winsize window_size;
  ioctl(0, TIOCGWINSZ, &window_size);

  window_dimensions.first = window_size.ws_row;
  window_dimensions.second = window_size.ws_col;

  draw_border('X', 0, 0, window_dimensions.first, 0);
  draw_border('X', 0, window_dimensions.second, window_dimensions.first,
              window_dimensions.second);
  draw_border('X', 0, 0, 0, window_dimensions.second);
  draw_border('X', window_dimensions.first, 0, window_dimensions.first,
              window_dimensions.second);

  move_cursor(1, window_dimensions.second / 2);
  std::cout << std::flush;

  move_cursor(window_dimensions.first, window_dimensions.second);
  std::cout << std::flush;
}

void BasicView::render(std::list<Rabbit> &rabbits_list,
                       std::list<Snake> &snakes_list) {
  render();

  for (const auto &bunny : rabbits_list) draw_rabbit(bunny);

  for (const auto &serpent : snakes_list) draw_snake(serpent);

  int snake_count = 0;
  move_cursor(window_dimensions.first, 1);
  for (const auto &serpent : snakes_list) {
    if (serpent.is_controlled == true)
      std::cout << "snake(human)" << std::to_string(snake_count) << ":"
                << std::to_string(serpent.length) << "##";
    else
      std::cout << "snake(bot)" << std::to_string(snake_count) << ":"
                << std::to_string(serpent.length) << "##";
    snake_count++;
  }

  move_cursor(window_dimensions.first, window_dimensions.second);
  std::cout << std::flush;
}

void BasicView::draw_border(char symbol, int x_start, int y_start, int x_end,
                            int y_end) {
  move_cursor(0, 0);

  if (x_start == x_end) {
    for (int i = y_start; i < y_end; i++) {
      std::cout << symbol;
      move_cursor(x_start, i + 1);
    }
  }

  if (y_start == y_end) {
    for (int i = x_start; i < x_end; i++) {
      std::cout << symbol;
      move_cursor(i + 1, y_start);
    }
  }
}

void BasicView::set_color(int brightness, int color) {
  std::cout << "\e[" << brightness << ";" << color << "m";
}

void BasicView::move_cursor(int x_pos, int y_pos) {
  std::cout << "\e[" << x_pos << ";" << y_pos << "H";
}

void BasicView::print_game_name(std::string game_name) {
  std::cout << "Game: " << game_name;
}

void BasicView::draw_rabbit(const Rabbit &bunny) {
  move_cursor(bunny.xy.first, bunny.xy.second);
  std::cout << "R";
}

void BasicView::draw_snake(const Snake &serpent) {
  if (serpent.length != 0) {
    move_cursor(serpent.head.first, serpent.head.second);
    std::cout << "S";

    for (const auto &segment : serpent.body) {
      move_cursor(segment.first, segment.second);
      std::cout << "s";
    }

    move_cursor(serpent.tail.first, serpent.tail.second);
    std::cout << "s";
  }

  return;
}

void BasicView::bye_print() {
  printf("\033[H\033[J");
  printf("\033[%d;%dH Good bye!\n", window_dimensions.first / 2,
         window_dimensions.second / 2);
  move_cursor(window_dimensions.first, window_dimensions.second);
}

void BasicView::mainloop() {
  struct pollfd input = {0, POLLIN, 0};
  char buf[BUFSIZE] = "";

  int timeout = 1000 / UPS;
  for (int i = 0; i < 10000000; i++) {
    if (is_game_goes == false) break;

    auto first_time = std::chrono::system_clock::now();
    int n = poll(&input, 1, timeout);
    auto second_time = std::chrono::system_clock::now();
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(
                   second_time - first_time)
                   .count();
    timeout -= time;

    if (n == 0 || timeout <= 0) {
      for (const auto &ontime : ontimes) {
        static auto third_time1 = std::chrono::system_clock::now();
        static auto third_time2 = std::chrono::system_clock::now();

        ontime();
        timeout = 1000 / UPS;

        third_time2 = third_time1;
        third_time1 = std::chrono::system_clock::now();
        int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(
                        third_time2 - third_time1)
                        .count();
        move_cursor(0, 3 * window_dimensions.first / 4);
        std::cout << "FPS:" << 1000.0 / time2 << std::endl;
      }
    }

    if (n == 1) {
      int a = read(0, buf, sizeof(buf));
      static int direction = 0;

      for (int i = 0; i < a; i++) {
        switch (buf[i]) {
          case 'w':
          case 'W':
            if (direction != DOWN) {
              direction = UP;
            }
            break;

          case 's':
          case 'S':
            if (direction != UP) {
              direction = DOWN;
            }
            break;

          case 'a':
          case 'A':
            if (direction != RIGHT) {
              direction = LEFT;
            }
            break;

          case 'd':
          case 'D':
            if (direction != LEFT) {
              direction = RIGHT;
            }
            break;

          default:
            break;
        }

        for (const auto &onkey : onkeys) {
          onkey(direction);
        }
      }
      buf[0] = '\0';
    }
  }

  bye_print();
}

BasicView::BasicView() {
  struct winsize wins;
  ioctl(0, TIOCGWINSZ, &wins);
  window_dimensions.first = wins.ws_row;
  window_dimensions.second = wins.ws_col;

  struct termios term = {};
  tcgetattr(0, &term);
  old_term = term;

  term.c_lflag &= ~ECHO;
  term.c_lflag &= ~ICANON;

  tcsetattr(0, TCSANOW, &term);
}

BasicView::~BasicView() { tcsetattr(0, TCSANOW, &old_term); }
