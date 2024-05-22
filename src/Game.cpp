#include "Game.hpp"

#include <chrono>
#include <csignal>

void Game::set_name(char *buff) { strcpy(game_name, buff); }

void Game::generate_rabbits() {
  srand(time(NULL));
  Rabbit rabbit;

  for (int i = 0; i < num_of_rabbits; i++) {
    rabbit.xy.first = (rand() % (view.window_dimensions.first - 2)) + 2;
    rabbit.xy.second = (rand() % (view.window_dimensions.second - 2)) + 2;

    rabbits.push_back(rabbit);
  }
}

void Game::generate_snakes() {
  if (is_human == true) {
    Snake snake;
    snake.length = 2;
    snake.direction = RIGHT;
    snake.is_controlled = true;

    snake.head = {3, 2};
    snake.tail = {3, 1};

    snakes.push_back(snake);
  }

  Snake snake2;
  for (int i = 0; i < num_of_bots; i++) {
    snake2.length = 2;
    snake2.direction = RIGHT;
    snake2.is_controlled = false;
    snake2.is_bot = !(i % 3);

    int snake_head_first = (rand() % (view.window_dimensions.first - 3)) + 2;
    int snake_head_second = (rand() % (view.window_dimensions.second - 3)) + 2;

    snake2.head = {snake_head_first, snake_head_second};
    snake2.tail = {snake_head_first - 1, snake_head_second};

    snakes.push_back(snake2);
  }
}

void Game::update() {
  auto current_time = std::chrono::system_clock::now();
  for (auto snake = snakes.begin(); snake != snakes.end(); snake++) {
    for (auto rabbit = rabbits.begin(); rabbit != rabbits.end(); rabbit++) {
      if (snake->head == rabbit->xy) {
        loc buf_loc = {0, 0};
        snake->length++;
        snake->body.push_back(buf_loc);

        rabbit->xy.first = (rand() % (view.window_dimensions.first - 2)) + 2;
        rabbit->xy.second = (rand() % (view.window_dimensions.second - 2)) + 2;
      }
    }

    if (snake->head.first == 1 ||
        snake->head.first == view.window_dimensions.first ||
        snake->head.second == 1 ||
        snake->head.second == view.window_dimensions.second) {
      if (snake->length != 0) snake->last_length = snake->length;

      snake->length = 0;
      snake->head = {0, 0};
    }

    for (auto snake2 = snakes.begin(); snake2 != snakes.end(); snake2++) {
      if (snake != snake2) {
        if (snake->head == snake2->head) {
          if (snake->length != 0) snake->last_length = snake->length;

          snake->length = 0;
          snake->head = {0, 0};

          if (snake2->length != 0) snake2->last_length = snake2->length;

          snake2->length = 0;
          snake2->head = {0, 0};
        }
      }

      for (const auto &body : snake2->body) {
        if (snake->head == body) {
          if (snake->length != 0) snake->last_length = snake->length;

          snake->length = 0;
          snake->head = {0, 0};
        }
      }
    }

    update_snake(snake);
  }

  for (auto snake : snakes) {
    if (snake.length != 0) {
      view.render(rabbits, snakes);
      return;
    }
  }
  end_game();
}

void Game::update_snake(std::list<Snake>::iterator snake) {
  int prev_x = snake->head.first;
  int prev_y = snake->head.second;
  int prev_2x = 0;
  int prev_2y = 0;

  switch (snake->direction) {
    case UP:
      snake->head.first--;
      break;

    case DOWN:
      snake->head.first++;
      break;

    case LEFT:
      snake->head.second--;
      break;

    case RIGHT:
      snake->head.second++;
      break;

    default:
      break;
  }

  for (auto part = snake->body.begin(); part != snake->body.end(); part++) {
    prev_2x = part->first;
    prev_2y = part->second;

    part->first = prev_x;
    part->second = prev_y;

    prev_x = prev_2x;
    prev_y = prev_2y;
  }

  snake->tail.first = prev_x;
  snake->tail.second = prev_y;
}

void Game::end_game() { view.is_game_goes = false; }

void Game::set_field() {
  for (auto &line : field)
    for (auto &cell : line) cell = 0;

  for (int i = 0; i <= view.window_dimensions.first; i++) {
    field[i][0] = 1;
    field[i][view.window_dimensions.second] = 1;
  }

  for (int i = 0; i <= view.window_dimensions.second; i++) {
    field[0][i] = 1;
    field[view.window_dimensions.first][i] = 1;
  }

  for (const auto &snake : snakes) {
    if (snake.head != loc{0, 0} && snake.length != 0) {
      field[snake.head.first][snake.head.second] = 1;

      for (const auto &body : snake.body) field[body.first][body.second] = 1;

      field[snake.tail.first][snake.tail.second] = 1;
    }
  }

  for (const auto &rabbit : rabbits) {
    field[rabbit.xy.first][rabbit.xy.second] = -1;
  }
}