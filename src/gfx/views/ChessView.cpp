#pragma once

#include "gfx/MainView.h"
#include "gfx/ViewManager.h"

#include "games/Chess.h"

using namespace ui;


class ChessRenderer : public GameRenderer
{
private:
  point_t cellHover;
  point_t margin;
  coord_t cs; // cell size

public:
  ChessRenderer();

  void render(ViewManager* gvm) override;
  void mouseMoved(point_t p) override;
};

GameRenderer* irenderer = new ChessRenderer();


ChessRenderer::ChessRenderer() : GameRenderer(), margin({ 12, 24 }), cs(24)
{
  cellHover = { -1, -1 };

}

void ChessRenderer::render(ViewManager* gvm)
{
  gvm->clear({ 255, 255, 255 });
  gvm->grid(margin.x, margin.y, games::chess::Board::WIDTH, games::chess::Board::HEIGHT, { cs, cs }, { 0, 0, 0 });

  static std::string files[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

  for (auto i = 0; i < 8; ++i)
    gvm->text(files[i], margin.x + cs/2 + (cs * i), 5, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);


}

void ChessRenderer::mouseMoved(point_t p)
{
  auto x = (p.x - margin.x) / cs, y = (p.y - margin.y) / cs;

  if (p.x >= margin.x && p.y >= margin.y && x >= 0 && x < games::chess::Board::WIDTH && y >= 0 && y < games::chess::Board::HEIGHT)
  {
    cellHover = { x, y };
  }
  else
    cellHover = { -1, -1 };
}