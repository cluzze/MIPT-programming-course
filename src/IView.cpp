#include "IView.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

#include <functional>

#include "BasicView.hpp"
#include "GraphicsView.hpp"

IView *IView::view = NULL;
IView *IView::get(std::string s) {
  if (view) return view;

  if (s == "text")
    view = new BasicView;
  else
    view = new GraphicsView;

  return view;
}

IView::IView() {
  struct winsize wins;
  ioctl(0, TIOCGWINSZ, &wins);
  window_dimensions.first = wins.ws_row;
  window_dimensions.second = wins.ws_col;
}

void IView::set_onkey(std::function<void(int)> f) { onkeys.push_back(f); }

void IView::set_ontimes(std::function<void()> f) { ontimes.push_back(f); }

void IView::set_draw_without_update(std::function<void()> f) {
  draw_without_update = f;
}

IView::~IView() {}
