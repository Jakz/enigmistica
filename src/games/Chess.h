#pragma once

#include "Common.h"

#include <array>
#include <unordered_set>

namespace games
{
  template<coord_t W, coord_t H, typename T>
  class Board
  {
  protected:
    std::array<T, W*H> board;

  public:
    T& get(coord_t x, coord_t y) { return board[x + y * W]; }
    const T& get(coord_t x, coord_t y) const { return board[x + y * W]; }

    coord_t width() const { return W; }
    coord_t height() const { return H; }

    coord_t firstRow() const { return 0; }
    coord_t lastRow() const { return H - 1; }

    coord_t firstColumn() const { return 0; }
    coord_t lastColumn() const { return W - 1; }

    typename decltype(board)::iterator begin() { return std::begin(board); }
    typename decltype(board)::iterator end() { return std::end(board); }
    typename decltype(board)::const_iterator begin() const { return std::begin(board); }
    typename decltype(board)::const_iterator end() const { return std::end(board); }

    static constexpr coord_t WIDTH = W;
    static constexpr coord_t HEIGHT = H;

    using Piece = T;
  };

  using Move = point_t;
  using MoveSet = std::unordered_set<point_t, point_t::hash>;

  class MoveResult
  {
  public:
    bool valid;

    MoveResult() : valid(true) { }
    MoveResult(bool valid) : valid(valid) { }

    operator bool() const { return valid; }
  };

  template<typename B>
  class BoardGame
  {
  public:
    using Board = B;
    using Piece = typename B::Piece;

  protected:
    B board;

  public:
    BoardGame() { }

    typename B::Piece& get(point_t p) { return board.get(p.x, p.y); }
    const typename B::Piece& get(point_t p) const { return board.get(p.x, p.y); }

    bool isValid(point_t p) const { return p.x >= 0 && p.x < board.width() && p.y >= 0 && p.y < board.height(); }

    virtual void resetBoard() = 0;
    virtual MoveResult pieceMoved(const Piece& piece, point_t from, point_t to) = 0;
    virtual MoveSet allowedMoves(const Piece& piece, point_t from) = 0;


  };

  namespace chess
  {
    struct Piece
    {
      enum class Type
      {
        Pawn, Rook, Bishop, Castle, Queen, King
      };

      enum class Color { White, Black };

      Type type;
      Color color;
      bool hasMoved;
      bool present;

      Piece() : present(false) { }
      Piece(Type type, Color color) : hasMoved(false), present(true), type(type), color(color) { }

      bool isWhite() const { return color == Color::White; }
      bool isEmpty() const { return !present; }

    };

    class Chess : public BoardGame<games::Board<8, 8, Piece>>
    {
    protected:


    public:
      void resetBoard() override
      {
        std::array<Piece::Type, 8> row = {
          Piece::Type::Castle, Piece::Type::Rook, Piece::Type::Bishop, Piece::Type::Queen,
          Piece::Type::King, Piece::Type::Bishop, Piece::Type::Rook, Piece::Type::Castle
        };

        std::fill(board.begin(), board.end(), Piece());

        for (auto i = 0; i < row.size(); ++i)
        {
          board.get(i, board.firstRow()) = { row[i], Piece::Color::White };
          board.get(i, board.firstRow() + 1) = { Piece::Type::Pawn, Piece::Color::White };

          board.get(i, board.lastRow()) = { row[i], Piece::Color::Black };
          board.get(i, board.lastRow() - 1) = { Piece::Type::Pawn, Piece::Color::Black };
        }
      }

      MoveResult pieceMoved(const Piece& piece, point_t from, point_t to) override
      {
        auto moves = allowedMoves(piece, from);

        //TODO: inefficient, allowedMoves called also from UI
        if (moves.find(to) != moves.end())
        {
          get(to) = piece;
          get(to).hasMoved = true;
          return MoveResult();
        }
        else
          return MoveResult(false);
      }

      MoveSet allowedMoves(const Piece& piece, point_t from) override
      {
        MoveSet moves;

        if (piece.type == Piece::Type::Pawn)
        {
          coord_t dy = piece.isWhite() ? +1 : -1;

          point_t next = from + point_t(0, dy);

          if (isValid(next) && get(next).isEmpty())
            moves.insert(next);

          if (!piece.hasMoved)
          {
            point_t next2 = from + point_t(0, 2*dy);

            if (isValid(next2)  && get(next).isEmpty() && get(next2).isEmpty())
              moves.insert(next2);
          }
        }

        return moves;
      }
    };
  }
}
