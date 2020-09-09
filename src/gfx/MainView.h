#pragma once

#include "ViewManager.h"
#include "Common.h"

struct ObjectGfx;

namespace ui
{
  class MainView : public View
  {
  private:
    point_t mouse;
    point_t margin;
    coord_t cs; // cell size

  public:
    MainView(ViewManager* gvm);

    void render() override;
    void handleKeyboardEvent(const SDL_Event& event) override;
    void handleMouseEvent(const SDL_Event& event) override;
  };

  class KeyboardView : public View
  {
  private:
    enum ButtonState { Normal, Hover };
    
    utf8_string value;

    point_t selected;

    void drawKeyButton(utf8_char character, point_t position, ButtonState state);

    point_t characterForCoordinate(point_t position);

  public:
    KeyboardView(ViewManager* gvm);

    void activate(bool full) override;

    void render() override;
    void handleKeyboardEvent(const SDL_Event& event) override;
    void handleMouseEvent(const SDL_Event& event) override;
  };
}
