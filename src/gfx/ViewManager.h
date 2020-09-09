#pragma once

#include "SdlHelper.h"

#include <array>
#include <vector>

namespace ui
{
  class ViewManager;
  
  class View
  {
  protected:
    ViewManager* gvm;
    View(ViewManager* gvm) : gvm(gvm) { }

  public:
    virtual void activate(bool full = true) { }

    virtual void render() = 0;
    virtual void handleKeyboardEvent(const SDL_Event& event) = 0;
    virtual void handleMouseEvent(const SDL_Event& event) = 0;
  };

  enum TextAlign
  {
    LEFT, CENTER, RIGHT
  };

  class MainView;
  class KeyboardView;

  class ViewManager : public SDL<ViewManager, ViewManager>
  {
  public:
    using view_t = View;

    SDL_Texture* _font;

  private:
    MainView* _mainView;
    KeyboardView* _keyboardView;

    std::vector<view_t*> _stack;

  public:
    ViewManager();

    bool loadData();

    void handleKeyboardEvent(const SDL_Event& event, bool press);
    void handleMouseEvent(const SDL_Event& event);
    void render();

    void deinit();

    void push(view_t* view) { _stack.push_back(view); view->activate(false); }
    void pop() { _stack.pop_back(); }
    void change(view_t* view) { if (!_stack.empty()) pop(); push(view); view->activate(); }

    SDL_Texture* font() { return _font; }

    int32_t textWidth(const std::string& text, float scale = 2.0f) const { return text.length() * scale * 4; }
    void text(const std::string& text, int32_t x, int32_t y, SDL_Color color, TextAlign align, float scale = 2.0f);
    void text(const std::string& text, int32_t x, int32_t y);
  };
}

