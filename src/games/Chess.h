#pragma once

#include "Common.h"

namespace games
{
  namespace chess
  {
    enum class Piece
    {
      Pawn, Rook, Bishop, Castle, Queen, King
    };
    
    struct BoardCell
    {
      Piece piece;
    };

    class Board
    {
    public:
      static constexpr coord_t WIDTH = 8;
      static constexpr coord_t HEIGHT = 8;

    private:

    public:
      Board();
    };
  }
}