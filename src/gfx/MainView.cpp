#pragma once

#include "MainView.h"
#include "ViewManager.h"

#include "games/Crossword.h"

using namespace ui;

extern GameRenderer* irenderer;

MainView::MainView(ViewManager* gvm) : View(gvm)
{
  this->renderer = irenderer;
}

void MainView::render()
{
  if (renderer)
    renderer->render(gvm);
  else
    gvm->clear({ 255, 255, 255 });
}

void MainView::handleKeyboardEvent(const SDL_Event& event)
{
  if (event.type == SDL_KEYDOWN)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE: gvm->exit(); break;
    }
  }
}

void MainView::handleMouseEvent(const SDL_Event& event)
{
  if (event.type == SDL_MOUSEMOTION)
  {
    if (renderer)
      renderer->mouseMoved({ event.motion.x, event.motion.y });
    
    //mouse.x = event.motion.x;
    //mouse.y = event.motion.y;
  }
  else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
  {
    auto bt = event.button.button;
    
    if (renderer)
      //TODO: add other buttons
      renderer->mouseButton({ event.button.x, event.button.y }, ui::MouseButton::Left, event.type == SDL_MOUSEBUTTONDOWN);
  }
}