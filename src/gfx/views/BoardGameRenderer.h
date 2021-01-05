#include "gfx/MainView.h"
#include "gfx/ViewManager.h"

#include "games/Chess.h"

namespace ui
{
  template<typename Game, typename Renderer>
  class BoardGameRenderer : public ui::GameRenderer
  {
  protected:
    Game game;
    Renderer pieceRenderer;

    using Board = typename Game::Board;
    using Move = typename Game::Move;
    using T = typename Game::Piece;

    struct
    {
      bool valid;
      point_t position;
      point_t cell;
    } mouse;

    struct
    {
      bool present;
      point_t from;
      T piece;
    } held;

    games::MoveSet<Move> availableMoves;
    games::PlayerMoveSet<Move> availableMovesForPlayer;

    point_t margin;
    coord_t cs; // cell size
    bool flipped = true;


  public:
    BoardGameRenderer();

    void render(ViewManager* gvm) override;
    void mouseMoved(point_t p) override;
    void mouseButton(point_t p, MouseButton button, bool pressed) override;
  };

  template<typename T, typename Renderer>
  BoardGameRenderer<T, Renderer>::BoardGameRenderer() : GameRenderer(), margin({ 12, 24 }), cs(24), mouse({ false, { 0,0}, { 0, 0} }), held({ false })
  {
    game.resetBoard();
    availableMovesForPlayer = game.allowedMoveSetForPlayer(game.currentPlayer());
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::render(ViewManager* gvm)
  {
    const auto boardSize = game.boardSize();
    const auto BW = boardSize.w;
    const auto BH = boardSize.h;

    gvm->clear({ 255, 255, 255 });
    gvm->grid(margin.x, margin.y, BW, BH, { cs, cs }, { 0, 0, 0 });

    static std::string ranks[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
    static std::string files[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

    for (auto i = 0; i < BW; ++i)
    {
      gvm->text(files[i], margin.x + cs / 2 + (cs * i) + 1, margin.y - 10, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
      gvm->text(files[i], margin.x + cs / 2 + (cs * i) + 1, margin.y + cs * BH + 4, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
    }

    for (auto i = 0; i < BH; ++i)
    {
      auto j = flipped ? BH - i - 1 : i;
      gvm->text(ranks[j], margin.x - 6, margin.y + cs / 2 + (cs * i) - 2, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
      gvm->text(ranks[j], margin.x + cs * BW + 7, margin.y + cs / 2 + (cs * i) - 2, { 180, 180, 180 }, TextAlign::CENTER, 1.0f);
    }

    for (auto x = 0; x < BW; ++x)
      for (auto y = 0; y < BH; ++y)
      {
        point_t base = point_t(margin.x + x * cs, margin.y + (flipped ? (BH - y - 1) : y) * cs);
        const auto coord = point_t(x, y);

        if ((y + x) % 2 == 1)
          gvm->fillRect({ base.x + 1, base.y + 1, cs - 1, cs - 1 }, color_t{ 80, 80, 80 });

        auto it = std::find_if(availableMoves.begin(), availableMoves.end(), [&coord](const Move& move) { return move.endsOn(coord); });

        if (it != availableMoves.end())
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 0, 220, 0 });
        else if (held.present && held.from == coord)
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 220, 220, 0 });
        if (!held.present && availableMovesForPlayer.find(coord) != availableMovesForPlayer.end())
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 0, 220, 0 });

        
        if (mouse.cell == coord)
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 220, 0, 0 });

        const auto& cell = game.get(coord);

        if (cell.present)
          pieceRenderer.render(gvm, base + cs / 2, cell);
      }

    if (held.present)
    {
      pieceRenderer.render(gvm, mouse.position, held.piece);
    }
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::mouseMoved(point_t p)
  {
    auto x = (p.x - margin.x) / cs, y = (p.y - margin.y) / cs;

    if (p.x >= margin.x && p.y >= margin.y && x >= 0 && x < game.boardSize().w && y >= 0 && y < game.boardSize().h)
    {
      if (flipped)
        mouse.cell = { x, game.boardSize().h - y - 1 };
      else
        mouse.cell = { x, y };
      mouse.valid = true;
    }
    else
    {
      mouse.cell = { -1, -1 };
      mouse.valid = false;
    }

    mouse.position = p;
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::mouseButton(point_t p, MouseButton button, bool pressed)
  {
    if (pressed && button == MouseButton::Left && mouse.valid)
    {
      if (!held.present)
      {
        if (game.canPickupPiece(mouse.cell))
        {
          auto& cell = game.get(mouse.cell);
          availableMoves = game.allowedMoves(cell, mouse.cell);
          if (!availableMoves.empty())
          {
            held = { true, mouse.cell, cell };
            cell = T();
          }
        }
      }
      else
      {
        /* cancel move */
        if (mouse.cell == held.from)
        {
          held.present = false;
          game.get(held.from) = held.piece;
          held.piece = T();
          availableMoves.clear();

        }
        else if (game.pieceMoved(held.piece, Move(held.from, mouse.cell)))
        {
          held.present = false;
          held.piece = T();
          availableMoves.clear();
          
          game.nextTurn();
          availableMovesForPlayer = game.allowedMoveSetForPlayer(game.currentPlayer());
        }
      }
    }
  }
}