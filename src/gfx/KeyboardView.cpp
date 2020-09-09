#include "MainView.h"
#include "ViewManager.h"

using namespace ui;



struct RowSpec
{
  utf8_string characters;
  pointf_t position;
};

/*
std::array<std::string, 3> rows = {
  "qwertyuiop", "asdfghjkl", "zxcvbnm"
};*/

std::array<RowSpec, 3> rows = { {
  { "qwertyuiop", { 0.05f, 0.30f } },
  { "asdfghjkl", { 0.075f, 0.43f } },
  { "zxcvbnm", { 0.10f, 0.56f } },
} };

static const rect_t bounds = { { 5, HEIGHT/2 }, { WIDTH - 10, HEIGHT/2 - 10 } };
static const float by = 0.1f;
static const size2d_t s = { 14, 14 };
static const size2d_t m = { 20, 20 };

KeyboardView::KeyboardView(ViewManager* gvm) : View(gvm)
{

}

void KeyboardView::render()
{
  gvm->fillRect(bounds, { 220, 220, 220, 220 });
  gvm->drawRect(bounds, { 0, 0, 0, 220 });

  auto j = 0;
  for (const auto& spec : rows)
  {
    for (s32 i = 0; i < spec.characters.size(); ++i)
    {
      point_t p = { bounds.origin.x + spec.position.x * bounds.size.w + m.w * i, bounds.origin.y + bounds.size.h * by /*spec.position.y * bounds.size.h*/ + j * m.h };
      drawKeyButton(spec.characters[i], p, selected == point_t{ i, j } ? ButtonState::Hover : ButtonState::Normal);
    }
    ++j;
  }

  gvm->text(value, 5, 5, { 0,0,0 }, ui::TextAlign::LEFT, 1.0f);
}

point_t KeyboardView::characterForCoordinate(point_t position)
{
  const auto x = position.x, y = position.y;

  auto j = 0;
  for (const auto& spec : rows)
  {
    const auto miny = bounds.origin.y + bounds.size.h * by + m.h * j;
    const auto maxy = miny + s.h;

    if (y >= miny && y < maxy)
    {
      const coord_t bx = x - (bounds.origin.x + spec.position.x * bounds.size.w);
      const coord_t i = bx / m.w, r = bx % m.h;

      if (i >= 0 && i < spec.characters.size() && r < s.w)
        return { i, j };
    }
    ++j;
  }

  return { -1, -1 };
}

void KeyboardView::activate(bool full)
{
  selected = { -1, -1 };
}

void KeyboardView::handleKeyboardEvent(const SDL_Event& event)
{

}

void KeyboardView::handleMouseEvent(const SDL_Event& event)
{
  if (event.type == SDL_MOUSEMOTION)
  {
    selected = characterForCoordinate({ event.motion.x, event.motion.y });
  }
  else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
  {
    selected = characterForCoordinate({ event.motion.x, event.motion.y });

    if (selected.x != -1)
    {
      auto c = rows[selected.y].characters[selected.x];
      value += c;
    }
  }
}

void KeyboardView::drawKeyButton(utf8_char character, point_t p, ButtonState state)
{
  gvm->fillRect({ p, s }, { 255, 255, 255 });
  gvm->drawRect({ p, s }, state == ButtonState::Hover ? color_t{ 255, 0, 0} : color_t{ 0, 0, 0 });
  gvm->text(std::string("") + character, p.x + s.w/2, p.y + s.h/4 - 1, { 0, 0, 0 }, ui::TextAlign::CENTER, 1.0f);
}