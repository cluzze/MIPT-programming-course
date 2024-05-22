#include "Control.hpp"
#include "Game.hpp"
#include "IView.hpp"

int main(const int argc, const char** argv) {
  IView* view;
  view = (argc > 1) ? (IView::get(argv[1])) : (IView::get("text"));

  int num_of_bots = 0;
  int num_of_rabbits = 0;
  bool is_human = false;

  if (argc > 2) num_of_bots = std::stoi(argv[2]);

  if (argc > 3) num_of_rabbits = std::stoi(argv[3]);

  if (argc > 4 && argv[4][0] == 'h') {
    is_human = true;
  }

  Game game(*view, num_of_bots, num_of_rabbits, is_human);

  std::list<Control*> control_list;
  auto current_snake = game.snakes.begin();

  for (int i = 0; i < num_of_bots; i++)
    control_list.push_back(std::move(new Control(game, *(current_snake++))));

  if (is_human)
    control_list.push_back(std::move(new Control(game, *(current_snake++))));

  view->mainloop();

  for (auto control_it = control_list.begin(); control_it != control_list.end();
       control_it++)
    delete *control_it;

  delete view;

  return 0;
}
