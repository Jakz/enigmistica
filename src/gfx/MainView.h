#pragma once

#include "ViewManager.h"
#include "Common.h"

namespace ui
{
  enum MouseButton { Left, Middle, Right };
  
  class GameRenderer
  {
  public:
    virtual void render(ViewManager* gvm) = 0;
    virtual void mouseMoved(point_t p) = 0;
    virtual void mouseButton(point_t p, MouseButton button, bool pressed) { }
  };
  
  class MainView : public View
  {
  private:
    GameRenderer* renderer;

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
