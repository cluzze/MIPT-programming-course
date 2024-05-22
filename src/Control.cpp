#include "Control.hpp"

#include <functional>
#include <queue>

void Control::get_user_input() {
  char buff[length_game_name] = "snake_def";
  game.set_name(buff);
}

Control::Control(Game &game_ref, Snake &snake_ref)
    : game(game_ref), snake(snake_ref) {
  auto key_pressed_lambda = [this](int key) { this->key_pressed(key); };
  game.view.set_onkey(key_pressed_lambda);

  auto timer_lambda = [this]() { this->timer(); };
  game.view.set_ontimes(timer_lambda);

  auto draw_without_update_lambda = [this]() { this->draw_without_update(); };
  game.view.set_draw_without_update(draw_without_update_lambda);
}

void Control::key_pressed(int input_key) {
  if (snake.is_controlled) snake.direction = input_key;
}

void Control::timer() {
  if (!snake.is_controlled) {
    if (snake.is_bot)
      silly_bot();
    else
      intelligent_bot();
  }
}

void Control::draw_without_update() {
  game.view.render(game.rabbits, game.snakes);
}

void Control::silly_bot() {
  int max_distance = 0;
  Rabbit farthest_rabbit;

  int distance = 0;

  for (const auto &bunny : game.rabbits) {
    distance = snake.head.dist(bunny.xy);

    if (distance > max_distance) {
      max_distance = distance;
      farthest_rabbit = bunny;
    }
  }

  if (snake.head.first != farthest_rabbit.xy.first) {
    if (snake.head.first < farthest_rabbit.xy.first) {
      if (snake.direction != UP)
        snake.direction = DOWN;
      else
        snake.direction = RIGHT;
    } else {
      if (snake.direction != DOWN)
        snake.direction = UP;
      else
        snake.direction = LEFT;
    }
  } else {
    if (snake.head.second != farthest_rabbit.xy.second) {
      if (snake.head.second < farthest_rabbit.xy.second) {
        if (snake.direction != LEFT)
          snake.direction = RIGHT;
        else
          snake.direction = DOWN;
      } else {
        if (snake.direction != RIGHT)
          snake.direction = LEFT;
        else
          snake.direction = UP;
      }
    }
  }
}

void Control::intelligent_bot() {
  if (snake.head == loc{0, 0}) return;

  game.set_field();

  std::vector<loc> generation;
  generation.push_back(snake.head);

  loc target_rabbit(-1, -1);
  target_rabbit = next_generation(generation);

  if (target_rabbit != loc{0, 0}) {
    loc previous_location = {};
    for (loc current_location = target_rabbit;
         current_location != snake.head && current_location != loc{0, 0};) {
      switch (game.field[current_location.first][current_location.second]) {
        case RIGHT + 1:
          previous_location = current_location;
          current_location.first += 1;
          break;

        case LEFT + 1:
          previous_location = current_location;
          current_location.first -= 1;
          break;

        case DOWN + 1:
          previous_location = current_location;
          current_location.second += 1;
          break;

        case UP + 1:
          previous_location = current_location;
          current_location.second -= 1;
          break;

        default:
          snake.direction = (snake.direction + 1) % 4;
          break;
      }
    }

    if (previous_location.first - snake.head.first == 1) snake.direction = DOWN;

    if (previous_location.first - snake.head.first == -1) snake.direction = UP;

    if (previous_location.second - snake.head.second == 1)
      snake.direction = RIGHT;

    if (previous_location.second - snake.head.second == -1)
      snake.direction = LEFT;
  }
}

loc Control::next_generation(std::vector<loc> &generation) {
  if (generation.empty() || snake.head == loc{0, 0}) return {0, 0};

  std::vector<loc> generation2;

  for (const auto &entity : generation) {
    if (in_bounds(entity.first - 1, entity.second) &&
        game.field[entity.first - 1][entity.second] == 0) {
      game.field[entity.first - 1][entity.second] = RIGHT + 1;
      generation2.push_back(loc(entity.first - 1, entity.second));
    }
    if (in_bounds(entity.first + 1, entity.second) &&
        game.field[entity.first + 1][entity.second] == 0) {
      game.field[entity.first + 1][entity.second] = LEFT + 1;
      generation2.push_back(loc(entity.first + 1, entity.second));
    }
    if (in_bounds(entity.first, entity.second - 1) &&
        game.field[entity.first][entity.second - 1] == 0) {
      game.field[entity.first][entity.second - 1] = DOWN + 1;
      generation2.push_back(loc(entity.first, entity.second - 1));
    }
    if (in_bounds(entity.first, entity.second + 1) &&
        game.field[entity.first][entity.second + 1] == 0) {
      game.field[entity.first][entity.second + 1] = UP + 1;
      generation2.push_back(loc(entity.first, entity.second + 1));
    }

    if (in_bounds(entity.first - 1, entity.second) &&
        game.field[entity.first - 1][entity.second] == -1) {
      game.field[entity.first - 1][entity.second] = RIGHT + 1;
      return {entity.first - 1, entity.second};
    }

    if (in_bounds(entity.first + 1, entity.second) &&
        game.field[entity.first + 1][entity.second] == -1) {
      game.field[entity.first + 1][entity.second] = LEFT + 1;
      return {entity.first + 1, entity.second};
    }

    if (in_bounds(entity.first, entity.second - 1) &&
        game.field[entity.first][entity.second - 1] == -1) {
      game.field[entity.first][entity.second - 1] = DOWN + 1;
      return {entity.first, entity.second - 1};
    }

    if (in_bounds(entity.first, entity.second + 1) &&
        game.field[entity.first][entity.second + 1] == -1) {
      game.field[entity.first][entity.second + 1] = UP + 1;
      return {entity.first, entity.second + 1};
    }
  }

  return next_generation(generation2);
}

bool Control::in_bounds(int length1, int length2) {
  return length1 >= 0 && length1 <= game.view.window_dimensions.first &&
         length2 >= 0 && length2 <= game.view.window_dimensions.second;
}