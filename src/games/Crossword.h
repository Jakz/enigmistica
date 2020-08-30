#pragma once

#include "Common.h"

#include <vector>

namespace games
{
  class Game
  {

  };

  struct WordDefinition
  {
    std::string text;
    std::string hint;
  };

  using coord_t = ::coord_t;
  using Position = ::point_t;
  using Size = ::size2d_t;

  enum class Dir { Hor, Ver };

  Position operator+(const Position& position, Dir dir)
  {
    if (dir == Dir::Hor)
      return { position.x + 1, position.y };
    else
      return { position.x, position.y + 1 };
  }

  Position& operator+=(Position& position, Dir dir)
  {
    if (dir == Dir::Hor)
      ++position.x;
    else
      ++position.y;

    return position;
  }

  struct CrosswordDefinition
  {
    WordDefinition definition;
    Position position;
    Dir orientation;
  };

  class CrosswordScheme : public Game
  {
  private:
    Size _size;
    std::vector<CrosswordDefinition> _definitions;
    HeapBitSet<uint_fast32_t> _filledCells;

  public:
    CrosswordScheme(s32 w, s32 h) : _size({ w, h }) { }

    void addDefinition(s32 x, s32 y, Dir dir, const std::string& text, const std::string& hint)
    {
      _definitions.push_back({ { text, hint }, { x, y }, dir });
    }

    void buildFilled()
    {
      _filledCells.init(_size.w * _size.h);

      for (const auto& def : _definitions)
      {
        auto p = def.position;

        for (auto i = 0; i < def.definition.text.size(); ++i)
        {
          _filledCells.set(p.y * _size.w + p.x);
          p += def.orientation;
        }
      }

      _filledCells.flip();
    }


    const auto& definitions() const { return _definitions; }
    bool isFilled(coord_t x, coord_t y) const { return _filledCells.isSet(y * _size.w + x); }
    coord_t width() const { return _size.w; }
    coord_t height() const { return _size.h; }
  };


}