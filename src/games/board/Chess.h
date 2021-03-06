#pragma once

#include "Common.h"
#include "games/board/Board.h"

namespace games
{
  namespace chess
  {
    struct Piece
    {
      enum class Type
      {
        Pawn, Rook, Bishop, Castle, Queen, King
      };


      Type type;
      Color color;
      bool hasMoved;
      bool present;

      Piece() : present(false) { }
      Piece(Type type, Color color) : hasMoved(false), present(true), type(type), color(color) { }

      bool isWhite() const { return color == Color::White; }
      bool isEmpty() const { return !present; }

      bool operator==(Color color) const { return present && this->color == color; }

    };

    struct Move
    {
      enum class Type { Movement, Castling, Promotion };
      
      Type type;
      point_t from;
      point_t to;

      Move(const point_t& from, const point_t& to, Type type = Type::Castling) : from(from), to(to), type(type) { }
      bool operator==(const Move& o) const { return type == o.type && from == o.from && to == o.to; }

      bool endsOn(const point_t& to) const { return this->to == to; }

      struct hash
      {
        size_t operator()(const Move& m) const { return point_t::hash()(m.to); }
      };
    };

    class Chess : public BoardGame<games::Board<8, 8, Piece, Move>>
    {
    protected:


    public:
      void resetBoard() override
      {
        std::array<Piece::Type, 8> row = {
          Piece::Type::Castle, Piece::Type::Rook, Piece::Type::Bishop, Piece::Type::Queen,
          Piece::Type::King, Piece::Type::Bishop, Piece::Type::Rook, Piece::Type::Castle
        };

        std::fill(_board.begin(), _board.end(), Piece());

        for (auto i = 0; i < row.size(); ++i)
        {
          _board.get(i, _board.firstRow()) = { row[i], Color::White };
          _board.get(i, _board.firstRow() + 1) = { Piece::Type::Pawn, Color::White };

          _board.get(i, _board.lastRow()) = { row[i], Color::Black };
          _board.get(i, _board.lastRow() - 1) = { Piece::Type::Pawn, Color::Black };
        }
      }

      MoveResult pieceMoved(const Piece& piece, const Move& move) override
      {
        auto moves = allowedMoves(piece, move.from);

        //TODO: inefficient, allowedMoves called also from UI
        if (moves.find(move) != moves.end())
        {
          get(move.to) = piece;
          get(move.to).hasMoved = true;

          if (move.type == Move::Type::Castling)
          {
            if (move.to.x == 2)
            {
              get({ 3, move.to.y }) = get({ _board.firstColumn(), move.to.y });
              get({ _board.firstColumn(), move.to.y }) = Piece();
              get({ 3, move.to.y }).hasMoved = true;
            }
            else if (move.to.x == 6)
            {
              get({ 5, move.to.y }) = get({ _board.lastColumn(), move.to.y });
              get({ _board.lastColumn(), move.to.y }) = Piece();
              get({ 5, move.to.y }).hasMoved = true;
            }
          }


          return MoveResult();
        }
        else
          return MoveResult(false);
      }

      MoveSet<Move> allowedMoves(const Piece& piece, point_t from) override
      {
        MoveSet<Move> moves;

        if (piece.type == Piece::Type::Pawn)
        {
          coord_t dy = piece.isWhite() ? +1 : -1;

          point_t next = from + point_t(0, dy);

          if (isValid(next) && get(next).isEmpty())
            moves.insert(Move(from, next));

          if (!piece.hasMoved)
          {
            point_t next2 = from + point_t(0, 2*dy);

            if (isValid(next2)  && get(next).isEmpty() && get(next2).isEmpty())
              moves.insert(Move(from, next2));
          }

          auto& c1 = get(from + point_t(-1, dy));
          auto& c2 = get(from + point_t(+1, dy));

          if (c1.present && c1.color != piece.color)
            moves.insert(Move(from, from + point_t(-1, dy)));

          if (c2.present && c2.color != piece.color)
            moves.insert(Move(from, from + point_t(+1, dy)));
        }
        
        if (piece.type == Piece::Type::Castle || piece.type == Piece::Type::Queen)
        {
          const std::array<point_t, 4> delta =
          {
            point_t(-1, 0), point_t(1, 0),
            point_t(0, -1), point_t(0, 1)
          };

          for (const auto& d : delta)
          {
            point_t next = from + d;

            bool wasEmpty = true;
            while (isValid(next) && (get(next).isEmpty() || (wasEmpty && get(next).color != piece.color)))
            {
              wasEmpty = get(next).isEmpty();
              moves.insert(Move(from, next));
              next += d;

              if (!wasEmpty)
                break;
            }
          }
        }

        if (piece.type == Piece::Type::Bishop || piece.type == Piece::Type::Queen)
        {
          const std::array<point_t, 4> delta =
          {
            point_t(-1, -1), point_t(+1, -1),
            point_t(-1, +1), point_t(+1, +1)
          };

          for (const auto& d : delta)
          {
            point_t next = from + d;

            bool wasEmpty = true;
            while (isValid(next) && (get(next).isEmpty() || (wasEmpty && get(next).color != piece.color)))
            {
              wasEmpty = get(next).isEmpty();
              moves.insert(Move(from, next));
              next += d;

              if (!wasEmpty)
                break;
            }
          }
        }

        if (piece.type == Piece::Type::King)
        {
          for (coord_t dx = -1; dx <= 1; ++dx)
            for (coord_t dy = -1; dy <= 1; ++dy)
            {
              if (dx != 0 || dy != 0)
              {
                point_t next = from + point_t(dx, dy);
                if (isValid(next) && (get(next).isEmpty() || get(next).color != piece.color))
                  moves.insert(Move(from, next));
              }
            }

          /* castling */
          if (!piece.hasMoved)
          {
            std::array<coord_t, 2> sides = { _board.firstColumn(), _board.lastColumn() };
            for (coord_t x : sides)
            {
              Piece& castle = get({ x, from.y });

              if (!castle.isEmpty() && castle == piece.color && castle.type == Piece::Type::Castle && !castle.hasMoved)
              {
                bool allEmpty = true;
                for (coord_t xx = std::min(x, from.x) + 1; xx < std::max(x, from.x); ++xx)
                  allEmpty &= get({ xx, from.y }).isEmpty();
                //TODO: check king won't get mated
                if (allEmpty)
                  moves.insert(Move(from, { x == 0 ? from.x - 2 : from.x + 2, from.y }, Move::Type::Castling));
              }
            }
          }
        }

        if (piece.type == Piece::Type::Rook)
        {
          std::array<point_t, 8> deltas = {
            point_t(-2, -1), point_t(-2, +1), point_t(+2, -1), point_t(+2, +1),
            point_t(-1, -2), point_t(+1, -2), point_t(-1, +2), point_t(+1, +2)
          };

          for (const auto& delta : deltas)
          {
            point_t next = from + delta;

            if (isValid(next) && (get(next).isEmpty() || get(next).color != piece.color))
              moves.insert(Move(from, next));
          }
        }

        return moves;
      }

      bool canPickupPiece(point_t from) override
      {
        return isValid(from) && get(from).present && get(from).color == _player->color;
      }
    };
  }
}
