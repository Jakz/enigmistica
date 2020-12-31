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

  SDL_Texture* pieces;

public:
  ChessRenderer();

  void render(ViewManager* gvm) override;
  void mouseMoved(point_t p) override;
};

GameRenderer* irenderer = new ChessRenderer();

games::chess::Board board;


ChessRenderer::ChessRenderer() : GameRenderer(), margin({ 12, 24 }), cs(24), pieces(nullptr)
{
  cellHover = { -1, -1 };

}

void ChessRenderer::render(ViewManager* gvm)
{
  if (!pieces)
    pieces = gvm->loadTexture("chess.png");
  
  constexpr auto BW = games::chess::Board::WIDTH;
  constexpr auto BH = games::chess::Board::HEIGHT;

  gvm->clear({ 255, 255, 255 });
  gvm->grid(margin.x, margin.y, BW, BH, { cs, cs }, { 0, 0, 0 });

  static std::string ranks[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
  static std::string files[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

  for (auto i = 0; i < BW; ++i)
  {
    gvm->text(files[i], margin.x + cs / 2 + (cs * i) + 1, margin.y - 10, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
    gvm->text(files[i], margin.x + cs / 2 + (cs * i) + 1, margin.y + cs * BH + 4, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
  }

  for (auto i = 0; i < BH; ++i)
  {
    gvm->text(ranks[i], margin.x - 6, margin.y + cs / 2 + (cs * i) - 2, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
    gvm->text(ranks[i], margin.x + cs * BW + 7, margin.y + cs / 2 + (cs * i) - 2, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
  }

  for (auto x = 0; x < BW; ++x)
    for (auto y = 0; y < BH; ++y)
    {
      point_t base = { margin.x + x * cs, margin.y + y * cs };
      
      if ((y + x) % 2 == 1)
        gvm->fillRect({ base.x + 1, base.y + 1, cs - 1, cs - 1 }, { 80, 80, 80 });

      const auto& cell = board.get(x, y);

      using Piece = games::chess::Piece;

      const coord_t size = 16;
      SDL_Rect rect = { 0, 0, size, size };

      if (cell.present)
      {
        switch (cell.type) {
          case Piece::Type::Pawn: rect.x = 0; break;
          case Piece::Type::Castle: rect.x = 1; break;
          case Piece::Type::Rook: rect.x = 2; break;
          case Piece::Type::Bishop: rect.x = 3; break;
          case Piece::Type::Queen: rect.x = 4; break;
          case Piece::Type::King: rect.x = 5; break;
        }

        switch (cell.color) {
          case Piece::Color::White: rect.y = 0; break;
          case Piece::Color::Black: rect.y = 1; break;
        }

        rect.x *= size;
        rect.y *= size;

        gvm->blit(pieces, rect, base.x + cs/2 - size/2, base.y + cs/2 - size/2);
      }
    }

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