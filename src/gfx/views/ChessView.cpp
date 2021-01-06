#include "gfx/MainView.h"
#include "gfx/ViewManager.h"

#include "BoardGameRenderer.h"

#include "games/board/Chess.h"
#include "games/board/Checkers.h"

using namespace ui;

struct ChessPieceRenderer
{
  SDL_Texture* pieces;

  ChessPieceRenderer() : pieces(nullptr) { }

  void render(ViewManager* gvm, point_t p, const games::chess::Piece& piece, bool floating = false)
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
      case games::Color::White: rect.origin.y = 0; break;
      case games::Color::Black: rect.origin.y = 1; break;
    }

    rect.origin.x *= size;
    rect.origin.y *= size;

    if (floating)
    {
      rect_t src = { 96, 0, 16, 16 };
      gvm->blit(pieces, src, p.x - size / 2, p.y - size / 2);
      p.y -= 6;
    }

    gvm->blit(pieces, rect, p.x - size / 2, p.y - size / 2);
  }
};

using ChessRenderer = BoardGameRenderer<games::chess::Chess, ChessPieceRenderer>;



struct CheckersPieceRenderer
{
  SDL_Texture* pieces;

  CheckersPieceRenderer() : pieces(nullptr) { }

  void render(ViewManager* gvm, point_t p, const games::checkers::Piece& piece)
  {
    if (!pieces)
      pieces = gvm->loadTexture("checkers.png");

    using Piece = games::checkers::Piece;

    const coord_t size = 16;
    rect_t rect = rect_t(0, 0, size, size);

    if (piece.type == Piece::Type::King)
      rect.origin.x += size;
    
    if (piece.color == games::Color::Black)
      rect.origin.y += size;

    gvm->blit(pieces, rect, p.x - size / 2, p.y - size / 2);
  }
};

using CheckersRenderer = BoardGameRenderer<games::checkers::Game, CheckersPieceRenderer>;


GameRenderer* irenderer = new ChessRenderer();