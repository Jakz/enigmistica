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
    utf8_string text;
    utf8_string hint;
  };

  using coord_t = ::coord_t;
  using Position = ::point_t;
  using Size = ::size2d_t;

  enum class Dir { Hor, Ver };

  inline Position operator+(const Position& position, Dir dir)
  {
    if (dir == Dir::Hor)
      return { position.x + 1, position.y };
    else
      return { position.x, position.y + 1 };
  }

  inline Position& operator+=(Position& position, Dir dir)
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

  public:
    CrosswordScheme(s32 w, s32 h) : _size({ w, h }) { }

    void addDefinition(s32 x, s32 y, Dir dir, const utf8_string& text, const utf8_string& hint)
    {
      _definitions.push_back({ { text, hint }, { x, y }, dir });
    }

    const std::vector<CrosswordDefinition>& definitions() const { return _definitions; }
    coord_t width() const { return _size.w; }
    coord_t height() const { return _size.h; }
  };
}
