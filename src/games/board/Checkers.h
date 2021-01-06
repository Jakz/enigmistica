#pragma once

#include "Common.h"
#include "games/board/Board.h"

namespace games
{
  namespace checkers
  {
    struct Piece
    {
      enum class Type { Men, King };
      
      bool present;
      Type type;
      Color color;

      Piece() : present(false) { }
      Piece(Type type, Color color) : type(type), color(color) { }

      bool operator==(Color color) const { return present && this->color == color; }
    };

    using Move = point_t;
    
    class Game : public BoardGame<Board<8, 8, Piece, Move>>
    {
    public:

      void resetBoard() override
      {
        std::fill(_board.begin(), _board.end(), Piece());

        for (coord_t y = 0; y < _board.height(); ++y)
        {
          if (y == 3 || y == 4)
            continue;

          coord_t x = (y == 1 || y == 5 || y == 7) ? 1 : 0;
          
          for (; x < _board.width(); x += 2)
          {
            get(point_t(x, y)) = Piece(Piece::Type::Men, y < _board.height() / 2 ? Color::White : Color::Black);
          }
        }
      }

      MoveResult pieceMoved(const Piece& piece, const Move& move) override
      {
        return MoveResult();
      }

      MoveSet<Move> allowedMoves(const Piece& piece, point_t from) override
      {
        return MoveSet<Move>();
      }

      bool canPickupPiece(point_t from) override
      {
        return isValid(from) && get(from).present;
      }
    };
  }
}
