#include "ViewManager.h"

#include "MainView.h"

using namespace ui;

ui::ViewManager::ViewManager() : SDL<ui::ViewManager, ui::ViewManager>(*this, *this), _font(nullptr),
_mainView(new MainView(this)), _keyboardView(new KeyboardView(this))
{
  change(_mainView);
  //push(_keyboardView);
}

void ui::ViewManager::deinit()
{
  SDL_DestroyTexture(_font);

  SDL::deinit();
}

bool ui::ViewManager::loadData()
{
  _font = loadTexture("font.png");

  return true;
}

SDL_Texture* ui::ViewManager::loadTexture(const std::string& path)
{
  SDL_Surface* surface = IMG_Load(path.c_str());
  assert(surface);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(surface);

  return texture;
}

void ui::ViewManager::handleKeyboardEvent(const SDL_Event& event, bool press)
{
  if (!_stack.empty())
    _stack.back()->handleKeyboardEvent(event);
}

void ui::ViewManager::handleMouseEvent(const SDL_Event& event)
{
  if (!_stack.empty())
    _stack.back()->handleMouseEvent(event);
}


void ui::ViewManager::render()
{
  for (auto* view : _stack)
    view->render();
}

void ui::ViewManager::text(const std::string& text, int32_t x, int32_t y)
{
  const float scale = 1.0;
  constexpr int32_t GLYPHS_PER_ROW = 32;

  for (size_t i = 0; i < text.length(); ++i)
  {
    SDL_Rect src = { 6 * (text[i] % GLYPHS_PER_ROW), 9 * (text[i] / GLYPHS_PER_ROW), 5, 8 };
    SDL_Rect dest = { x + 6 * i * scale, y, 5 * scale, 8 * scale };
    SDL_RenderCopy(_renderer, _font, &src, &dest);
  }
}

void ViewManager::text(const std::string& text, int32_t x, int32_t y, SDL_Color color, TextAlign align, float scale)
{
  constexpr int32_t GLYPHS_PER_ROW = 32;

  const int32_t width = text.size() * 6 * scale;

  if (align == TextAlign::CENTER)
    x -= width / 2;
  else if (align == TextAlign::RIGHT)
    x -= width;

  SDL_SetTextureColorMod(_font, color.r, color.g, color.b);

  for (size_t i = 0; i < text.length(); ++i)
  {
    SDL_Rect src = { 6 * (text[i] % GLYPHS_PER_ROW), 9 * (text[i] / GLYPHS_PER_ROW), 5, 8 };
    SDL_Rect dest = { x + 6 * i * scale, y, 5 * scale, 8 * scale };
    SDL_RenderCopy(_renderer, _font, &src, &dest);
  }

  SDL_SetTextureColorMod(_font, 255, 255, 255);
}