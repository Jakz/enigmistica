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

  public:
    KeyboardView(ViewManager* gvm);

    void render() override;
    void handleKeyboardEvent(const SDL_Event& event) override;
    void handleMouseEvent(const SDL_Event& event) override;
  };
}
