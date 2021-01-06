#pragma once

#include "Common.h"

#include <array>
#include <unordered_set>
#include <unordered_map>

namespace games
{
  enum class Color { White, Black };

  template<coord_t W, coord_t H, typename T, typename M>
  class Board
  {
  protected:
    std::array<T, W*H> _board;

  public:
    T& get(coord_t x, coord_t y) { return _board[x + y * W]; }
    const T& get(coord_t x, coord_t y) const { return _board[x + y * W]; }

    coord_t width() const { return W; }
    coord_t height() const { return H; }

    coord_t firstRow() const { return 0; }
    coord_t lastRow() const { return H - 1; }

    coord_t firstColumn() const { return 0; }
    coord_t lastColumn() const { return W - 1; }

    typename decltype(_board)::iterator begin() { return std::begin(_board); }
    typename decltype(_board)::iterator end() { return std::end(_board); }
    typename decltype(_board)::const_iterator begin() const { return std::begin(_board); }
    typename decltype(_board)::const_iterator end() const { return std::end(_board); }

    using Move = M;
    using Piece = T;
  };

  struct Player
  {
    Color color;

    Player(Color color) : color(color) { }
  };

  template<typename M> using MoveSet = std::unordered_set<M, typename M::hash>;
  template<typename M> using PlayerMoveSet = std::unordered_map<point_t, MoveSet<M>, point_t::hash>;

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
    using Move = typename B::Move;

  protected:
    std::vector<Player> _players;
    decltype(_players)::iterator _player;
    B _board;

  public:
    BoardGame()
    {
      _players.push_back({ Color::White });
      _players.push_back({ Color::Black });
      _player = _players.begin();
    }

    const Board& board() const { return _board; }

    typename B::Piece& get(point_t p) { return _board.get(p.x, p.y); }
    const typename B::Piece& get(point_t p) const { return _board.get(p.x, p.y); }

    size2d_t boardSize() const { return size2d_t(_board.width(), _board.height()); }
    bool isValid(point_t p) const { return p.x >= 0 && p.x < _board.width() && p.y >= 0 && p.y < _board.height(); }

    void nextTurn() { ++_player; if (_player == _players.end()) _player = _players.begin(); }
    const Player& currentPlayer() { return *_player; }
    coord_t playerCount() const { return 2; }

    virtual void resetBoard() = 0;
    virtual bool canPickupPiece(point_t from) = 0;
    virtual MoveResult pieceMoved(const Piece& piece, const Move& move) = 0;
    virtual MoveSet<Move> allowedMoves(const Piece& piece, point_t from) = 0;

    virtual PlayerMoveSet<Move> allowedMoveSetForPlayer(const Player& player)
    {
      PlayerMoveSet<Move> set;

      for (int y = 0; y < _board.height(); ++y)
        for (int x = 0; x < _board.width(); ++x)
        {
          auto coord = point_t(x, y);
          if (get(coord) == player.color)
          {
            auto&& moves = allowedMoves(get(coord), coord);
            if (!moves.empty())
              set[coord] = moves;
          }
        }

      return set;
    }
  };
}