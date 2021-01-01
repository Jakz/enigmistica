#pragma once

#include "Common.h"

#include <array>

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

    auto width() const { return W; }
    auto height() const { return H; }

    auto firstRow() const { return 0; }
    auto lastRow() const { return H - 1; }

    auto firstColumn() const { return 0; }
    auto lastColumn() const { return W - 1; }

    auto begin() { return std::begin(board); }
    auto end() { return std::end(board); }
    auto begin() const { return std::begin(board); }
    auto end() const { return std::end(board); }

    static constexpr coord_t WIDTH = W;
    static constexpr coord_t HEIGHT = H;

    using Piece = T;
  };

  class MoveResult
  {
  public:
    bool valid;

    MoveResult() : valid(true) { }

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

    virtual void resetBoard() = 0;
    virtual MoveResult pieceMoved(const Piece& piece, point_t from, point_t to) = 0;


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

      bool present;
      Type type;
      Color color;

      Piece() : present(false) { }
      Piece(Type type, Color color) : present(true), type(type), color(color) { }
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


        std::fill(board.begin(), board.end(), Piece{});

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
        get(to) = piece;
        return MoveResult();
      }
    };
  }
}