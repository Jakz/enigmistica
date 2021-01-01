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

    static constexpr coord_t WIDTH = W;
    static constexpr coord_t HEIGHT = H;

    using piece_t = T;
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
    
    class Board : public games::Board<8, 8, Piece>
    {
    private:

    public:
      Board()
      {
        reset();
      }

      void reset()
      {
        std::array<Piece::Type, 8> row = {
          Piece::Type::Castle, Piece::Type::Rook, Piece::Type::Bishop, Piece::Type::Queen,
          Piece::Type::King, Piece::Type::Bishop, Piece::Type::Rook, Piece::Type::Castle
        };


        std::fill(board.begin(), board.end(), Piece{});

        for (auto i = 0; i < row.size(); ++i)
        {
          get(i, firstRow()) = { row[i], Piece::Color::White };
          get(i, firstRow() + 1) = { Piece::Type::Pawn, Piece::Color::White };

          get(i, lastRow()) = { row[i], Piece::Color::Black };
          get(i, lastRow() - 1) = { Piece::Type::Pawn, Piece::Color::Black };
        }
      }


    };
  }
}