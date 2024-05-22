#include "GraphicsView.hpp"

#include <chrono>

#include "Game.hpp"

const int BUFSIZE = 10;

GraphicsView::GraphicsView()
    : window(sf::VideoMode(150 * pixel_size + 300, 90 * pixel_size),
             "Game: Snake"),
      brick(sf::Vector2f(10.f, 10.f)) {
  window_dimensions.first = 150;
  window_dimensions.second = 90;
  brick.setFillColor(sf::Color::Green);
  brick.setPosition(window_dimensions.first * pixel_size / 2,
                    window_dimensions.second * pixel_size / 2);

  if (!rabbit_texture.loadFromFile("../assets/rabbit.png"))
    std::cout << "povialdjvldfbjkdf[p;bljkvp[;lgx;kbd;bvd;l]]" << std::endl;
  rabbit.setTexture(rabbit_texture);

  if (!snake_body_texture_human.loadFromFile("../assets/snakebody.jpg"))
    std::cout << "povialdjvldfbjkdf[p;bljkvp[;lgx;kbd;bvd;l]]" << std::endl;
  snake_body_human.setTexture(snake_body_texture_human);

  if (!snake_body_texture_stupid_bot.loadFromFile("../assets/snakebody2.png"))
    std::cout << "povialdjvldfbjkdf[p;bljkvp[;lgx;kbd;bvd;l]]" << std::endl;
  snake_body_stupid_bot.setTexture(snake_body_texture_stupid_bot);

  if (!snake_body_texture_smart_bot.loadFromFile("../assets/snakebody3.png"))
    std::cout << "povialdjvldfbjkdf[p;bljkvp[;lgx;kbd;bvd;l]]" << std::endl;
  snake_body_smart_bot.setTexture(snake_body_texture_smart_bot);

  if (!snake_head_texture.loadFromFile("../assets/snakehead.png"))
    std::cout << "povialdjvldfbjkdf[p;bljkvp[;lgx;kbd;bvd;l]]" << std::endl;
  snake_head.setTexture(snake_head_texture);

  text_box.setCharacterSize(24);
  if (!font.loadFromFile("../fonts/arial.ttf")) {
    std::cout << "No \"arial.ttf\" file in this directory" << std::endl;
  }
  text_box.setFont(font);
  text_box.setFillColor(sf::Color::Red);
  text_box.setPosition(window_dimensions.first * pixel_size + 2 * pixel_size,
                       3 * pixel_size);

  window.setFramerateLimit(frame);
}

GraphicsView::~GraphicsView() {}

void GraphicsView::render() {
  for (int i = 0; i < window_dimensions.first; i++) {
    brick.setPosition(i * pixel_size, 0);
    window.draw(brick);
    brick.setPosition(i * pixel_size,
                      window_dimensions.second * pixel_size - pixel_size);
    window.draw(brick);
  }

  for (int i = 0; i < window_dimensions.second; i++) {
    brick.setPosition(0, i * pixel_size);
    window.draw(brick);
    brick.setPosition(window_dimensions.first * pixel_size - pixel_size,
                      i * pixel_size);
    window.draw(brick);
  }
}

void GraphicsView::render(std::list<Rabbit> &rabbits,
                          std::list<Snake> &snakes) {
  render();

  for (const auto &rabbit : rabbits) draw_rabbit(rabbit);

  for (const auto &snake : snakes) draw_snake(snake);

  text_box.setPosition(window_dimensions.first * pixel_size + 2 * pixel_size,
                       3 * pixel_size);
  int snake_number = 0;
  for (const auto &snake : snakes) {
    if (snake.head == loc{0, 0} || snake.length == 0)
      text_box.setFillColor(sf::Color::Red);
    else
      text_box.setFillColor(sf::Color::Green);

    if (snake.is_controlled == true)
      text_box.setString("snake(human)" + std::to_string(snake_number) + ": " +
                         std::to_string(snake.length + snake.last_length));
    else
      text_box.setString("snake(bot)" + std::to_string(snake_number) + ": " +
                         std::to_string(snake.length + snake.last_length));
    window.draw(text_box);
    text_box.move(0, 3 * pixel_size);
    snake_number++;
  }

  std::string str = "";
  snake_number = 0;

  for (const auto &snake : snakes) {
    if (snake.is_controlled == true)
      str += "snake(human)" + std::to_string(snake_number++) + ": " +
             std::to_string(snake.last_length + snake.length) + "\n";
    else
      str += "snake(bot)" + std::to_string(snake_number++) + ": " +
             std::to_string(snake.last_length + snake.length) + "\n";
  }
  text_box.setString(str);

  return;
}

void GraphicsView::draw_rabbit(const Rabbit &rabbit_) {
  rabbit.setPosition((rabbit_.xy.first - 1) * pixel_size,
                     (rabbit_.xy.second - 1) * pixel_size);
  window.draw(rabbit);
}

void GraphicsView::draw_snake(const Snake &snake) {
  if (snake.is_controlled) {
    snake_body = snake_body_human;
  } else {
    if (snake.is_bot)
      snake_body = snake_body_stupid_bot;
    else
      snake_body = snake_body_smart_bot;
  }

  snake_head.setPosition((snake.head.first - 1) * pixel_size,
                         (snake.head.second - 1) * pixel_size);

  if (snake.direction == RIGHT) {
    snake_head.setRotation(-90.f);
    snake_head.move(0, pixel_size);
  }
  if (snake.direction == LEFT) {
    snake_head.setRotation(90.f);
    snake_head.move(pixel_size, 0);
  }
  if (snake.direction == DOWN) {
    snake_head.setRotation(180.f);
    snake_head.move(pixel_size, pixel_size);
  }

  window.draw(snake_head);
  snake_head.setRotation(0);

  for (const auto &part : snake.body) {
    snake_body.setPosition((part.first - 1) * pixel_size,
                           (part.second - 1) * pixel_size);
    window.draw(snake_body);
  }

  snake_body.setPosition((snake.tail.first - 1) * pixel_size,
                         (snake.tail.second - 1) * pixel_size);
  window.draw(snake_body);

  return;
}

void GraphicsView::print_game_name(std::string game_name) {
  std::cout << "(GraphicsView)Game: " << game_name << std::endl;
}

void GraphicsView::bye_print() {
  text_box.setPosition(window_dimensions.first * pixel_size / 2 - 150,
                       2 * pixel_size);
  std::string str = text_box.getString();
  text_box.setString(str);

  window.draw(text_box);
  return;
}

void GraphicsView::mainloop() {
  int direction = 0;
  int frame_number = 0;

  while (window.isOpen()) {
    auto time1 = std::chrono::system_clock::now();
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::A && direction != DOWN)
          direction = UP;

        if (event.key.code == sf::Keyboard::D && direction != UP)
          direction = DOWN;

        if (event.key.code == sf::Keyboard::W && direction != RIGHT)
          direction = LEFT;

        if (event.key.code == sf::Keyboard::S && direction != LEFT)
          direction = RIGHT;
      }
    }

    if (is_game_goes == false && (frame_number % (frame / UPS) == 0)) {
      bye_print();
      window.display();
      continue;
    }

    frame_number = (frame_number + 1) % frame;

    window.clear();
    if (frame_number % (frame / UPS) != 0) {
      for (const auto &onkey : onkeys) onkey(direction);

      draw_without_update();
    } else {
      for (const auto &ontime : ontimes) ontime();
    }

    window.display();

    auto time2 = std::chrono::system_clock::now();
    int time =
        std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1)
            .count();
    std::cout << 1000.0 / time << std::endl;
  }

  return;
}
