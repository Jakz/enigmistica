#pragma once

#include "MainView.h"
#include "ViewManager.h"

#include "games/Crossword.h"

using namespace ui;


namespace gfx
{
  enum class Status { Normal, Hidden, Blocked };

  struct CellStatus
  {
    char text; //TODO: utf8 support
    Status status;

    CellStatus() : text('\0'), status(Status::Blocked) { }
  };
  
  class CrosswordGfxStatus
  {
  private:
    s32 w;
    std::vector<CellStatus> status;

  public:
    CrosswordGfxStatus(s32 w, s32 h) : w(w)
    {
      status.resize(w*h);
    }

    CellStatus& at(s32 x, s32 y) { return status[y * w + x]; }
    const CellStatus& at(s32 x, s32 y) const { return status[y*w + x]; }

    void update(const games::CrosswordScheme* scheme)
    {
      for (const auto& def : scheme->definitions())
      {
        auto p = def.position;
        for (s32 i = 0; i < def.definition.text.size(); ++i)
        {
          at(p.x, p.y).text = def.definition.text[i];
          at(p.x, p.y).status = Status::Normal;

          p += def.orientation;
        }
      }
    }
  };
}


games::CrosswordScheme scheme = games::CrosswordScheme(13, 13);
gfx::CrosswordGfxStatus schemeStatus = gfx::CrosswordGfxStatus(13, 13);

MainView::MainView(ViewManager* gvm) : View(gvm), margin({ 1, 1 }), cs(14)
{
  mouse = { -1, -1 };

  scheme.addDefinition(0, 0, games::Dir::Hor, "casse", "Servono per imballare");
  scheme.addDefinition(6, 0, games::Dir::Hor, "iago", "Desta la gelosia di Otello");
  scheme.addDefinition(11, 0, games::Dir::Hor, "tr", "Filtri... senza fili");
  scheme.addDefinition(0, 1, games::Dir::Hor, "incidente", "Una sciagura stradale");

  scheme.addDefinition(0, 0, games::Dir::Ver, "cibo", "Cosa da mangiare");
  scheme.addDefinition(1, 0, games::Dir::Ver, "anonima", "Priva di firma");

  schemeStatus.update(&scheme);
}


void MainView::render()
{
  auto r = gvm->renderer();

  gvm->clear({ 255, 255, 255 });

  const auto w = scheme.width(), h = scheme.height();

  for (int32_t x = 0; x < w + 1; ++x)
    gvm->line(margin.x + x * cs, margin.y, margin.x + x * cs, margin.y + h * cs, { 0, 0, 0 });

  for (int32_t y = 0; y < h + 1; ++y)
    gvm->line(margin.x, margin.y + y * cs, margin.x + w * cs, margin.y + y * cs, { 0, 0, 0 });

  for (int32_t y = 0; y < h; ++y)
    for (int32_t x = 0; x < w; ++x)
    {
      const gfx::CellStatus& status = schemeStatus.at(x, y);

      if (status.status == gfx::Status::Blocked)
        gvm->fillRect(1 + x * cs + 2, 1 + y * cs + 2, cs - 3, cs - 3, { 40, 40, 40, 255 });
      else if (status.status == gfx::Status::Normal)
        gvm->text(std::string("") + status.text, x * cs + cs/2 + 1, y * cs + cs/4 + 1, { 0, 0, 0 }, ui::TextAlign::CENTER, 1.0f);
    }

  if (mouse.x != -1)
    gvm->drawRect(margin.x + cs * mouse.x, margin.y + cs * mouse.y, cs+1, cs+1, { 255, 0, 0 });
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
    auto x = (event.motion.x - margin.x) / cs, y = (event.motion.y - margin.y) / cs;

    if (event.motion.x >= margin.x && event.motion.y >= margin.y && x >= 0 && x < scheme.width() && y >= 0 && y < scheme.height())
    {
      mouse.x = x;
      mouse.y = y;
    }
    else
      mouse.x = mouse.y = -1;
    

    
    
    //mouse.x = event.motion.x;
    //mouse.y = event.motion.y;
  }
}