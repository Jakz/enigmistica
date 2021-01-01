#pragma once

#include "gfx/MainView.h"
#include "gfx/ViewManager.h"

#include "games/Chess.h"

using namespace ui;

struct ChessPieceRenderer
{
  SDL_Texture* pieces;

  ChessPieceRenderer() : pieces(nullptr) { }

  void render(ViewManager* gvm, point_t p, const games::chess::Piece& piece)
  {
    if (!pieces)
      pieces = gvm->loadTexture("chess.png");

    using Piece = games::chess::Piece;

    const coord_t size = 16;
    rect_t rect = { 0, 0, size, size };

    switch (piece.type) {
      case Piece::Type::Pawn: rect.origin.x = 0; break;
      case Piece::Type::Castle: rect.origin.x = 1; break;
      case Piece::Type::Rook: rect.origin.x = 2; break;
      case Piece::Type::Bishop: rect.origin.x = 3; break;
      case Piece::Type::Queen: rect.origin.x = 4; break;
      case Piece::Type::King: rect.origin.x = 5; break;
    }

    switch (piece.color) {
      case Piece::Color::White: rect.origin.y = 0; break;
      case Piece::Color::Black: rect.origin.y = 1; break;
    }

    rect.origin.x *= size;
    rect.origin.y *= size;

    gvm->blit(pieces, rect, p.x - size / 2, p.y - size / 2);
  }
};

class ChessRenderer : public GameRenderer
{
  using T = games::chess::Piece;
  using Renderer = ChessPieceRenderer;

private:

  games::chess::Board board;

  Renderer pieceRenderer;

  struct
  {
    bool valid;
    point_t position;
    point_t cell;
  } mouse;

  point_t margin;
  coord_t cs; // cell size

  T heldPiece;

public:
  ChessRenderer();

  void render(ViewManager* gvm) override;
  void mouseMoved(point_t p) override;
  void mouseButton(point_t p, MouseButton button, bool pressed) override;
};



GameRenderer* irenderer = new ChessRenderer();


ChessRenderer::ChessRenderer() : GameRenderer(), margin({ 12, 24 }), cs(24), mouse({ false, { 0,0}, { 0, 0} })
{

}

void ChessRenderer::render(ViewManager* gvm)
{

  
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

      if (mouse.cell == point_t{ x, y })
        gvm->drawRect({ base.x + 1, base.y + 1, cs - 1, cs - 1 }, { 220, 0, 0 });

      const auto& cell = board.get(x, y);

      if (cell.present)
      {
        pieceRenderer.render(gvm, base + cs / 2, cell);
      }
    }

  if (heldPiece.present)
  {
    pieceRenderer.render(gvm, mouse.position, heldPiece);
  }
}

void ChessRenderer::mouseMoved(point_t p)
{
  auto x = (p.x - margin.x) / cs, y = (p.y - margin.y) / cs;

  if (p.x >= margin.x && p.y >= margin.y && x >= 0 && x < games::chess::Board::WIDTH && y >= 0 && y < games::chess::Board::HEIGHT)
  {
    mouse.cell = { x, y };
    mouse.valid = true;
  }
  else
  {
    mouse.cell = { -1, -1 };
    mouse.valid = false;
  }

  mouse.position = p;
}


void ChessRenderer::mouseButton(point_t p, MouseButton button, bool pressed)
{
  if (pressed && button == MouseButton::Left && mouse.valid && !heldPiece.present)
  {
    auto& cell = board.get(mouse.cell.x,  mouse.cell.y);
    heldPiece = cell;
    cell = T();
  }
}