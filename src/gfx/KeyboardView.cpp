#include "MainView.h"
#include "ViewManager.h"

using namespace ui;



struct RowSpec
{
  std::string characters;
  pointf_t position;
};

/*
std::array<std::string, 3> rows = {
  "qwertyuiop", "asdfghjkl", "zxcvbnm"
};*/

std::array<RowSpec, 1> rows = { {
  { "qwertyuiop", { 0.05f, 0.30f } },
} };

rect_t bounds = { {5, 5}, {WIDTH - 10, HEIGHT - 10} };


KeyboardView::KeyboardView(ViewManager* gvm) : View(gvm)
{

}

void KeyboardView::render()
{
  gvm->fillRect(bounds, { 220, 220, 220, 220 });
  gvm->drawRect(bounds, { 0, 0, 0, 220 });

}

void KeyboardView::handleKeyboardEvent(const SDL_Event& event)
{

}

void KeyboardView::handleMouseEvent(const SDL_Event& event)
{

}
