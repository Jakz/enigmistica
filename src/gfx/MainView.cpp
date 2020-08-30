#pragma once

#include "MainView.h"
#include "ViewManager.h"

#include "games/Crossword.h"

using namespace ui;

games::CrosswordScheme scheme = games::CrosswordScheme(13, 13);


MainView::MainView(ViewManager* gvm) : gvm(gvm)
{
  mouse = { -1, -1 };

  scheme.addDefinition(0, 0, games::Dir::Hor, "casse", "Servono per imballare");
  scheme.addDefinition(0, 0, games::Dir::Ver, "cibo", "Cosa da mangiare");
  scheme.buildFilled();
}


void MainView::render()
{
  auto r = gvm->renderer();

  gvm->clear({ 255, 255, 255 });

  const auto w = scheme.width(), h = scheme.height();
  int32_t s = 14;
  point_t margin = { 1, 1 };

  for (int32_t x = 0; x < w + 1; ++x)
    gvm->line(margin.x + x * s, margin.y, margin.x + x * s, margin.y + h * s, { 0, 0, 0 });

  for (int32_t y = 0; y < h + 1; ++y)
    gvm->line(margin.x, margin.y + y * s, margin.x + w * s, margin.y + y * s, { 0, 0, 0 });

  for (int32_t y = 0; y < h; ++y)
    for (int32_t x = 0; x < w; ++x)
      if (scheme.isFilled(x, y))
        gvm->fillRect(1 + x * s + 2, 1 + y * s + 2, s - 3, s - 3, { 0, 0, 0 });

  for (const auto& def : scheme.definitions())
  {
    //TODO: utf8 support
    const auto len = def.definition.text.size();

    int x = def.position.x * s + margin.x + s/2;
    int y = def.position.y * s + margin.x + s/4;

    for (int i = 0; i < len; ++i)
    {
      gvm->text(def.definition.text.substr(i, 1), x, y, { 0, 0, 0 }, ui::TextAlign::CENTER, 1.0f);

      if (def.orientation == games::Dir::Hor)
        x += s;
      else
        y += s;      
    }
  }

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
    mouse.x = event.motion.x;
    mouse.y = event.motion.y;
  }
}