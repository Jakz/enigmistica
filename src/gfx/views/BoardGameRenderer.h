#include "gfx/MainView.h"
#include "gfx/ViewManager.h"

#include "games/board/Board.h"

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

    bool mouseMode = true;

    struct
    {
      bool valid;
      point_t position;
      point_t cell;
    } mouse;

    struct
    {
      bool valid;
      point_t cell;
    } gamepad;

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

    bool tryToPickupPieceAt(point_t coord);
    bool tryToDropPieceAt(point_t coord);

  public:
    BoardGameRenderer();

    void render(ViewManager* gvm) override;
    void mouseMoved(point_t p) override;
    void mouseButton(point_t p, MouseButton button, bool pressed) override;
    void gamepadButton(GamepadButton button, bool pressed) override;
  };

  template<typename T, typename Renderer>
  BoardGameRenderer<T, Renderer>::BoardGameRenderer() : GameRenderer(), margin({ 12, 24 }), cs(24), mouse({ false, { 0,0}, { 0, 0} }), gamepad({ false }), held({ false })
  {
    game.resetBoard();
    availableMovesForPlayer = game.allowedMoveSetForPlayer(game.currentPlayer());

    margin.x = WIDTH / 2 - cs * game.boardSize().w / 2;
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


        if (mouseMode && mouse.cell == coord)
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 220, 0, 0 });
        if (!mouseMode && gamepad.valid && gamepad.cell == coord)
          gvm->drawRect(rect_t(base.x + 1, base.y + 1, cs - 1, cs - 1), color_t{ 220, 0, 0 });


        const auto& cell = game.get(coord);

        if (cell.present)
          pieceRenderer.render(gvm, base + cs / 2, cell);
      }

    if (held.present)
    {
      if (mouseMode)
        pieceRenderer.render(gvm, mouse.position, held.piece);
      else
      {
        point_t base = point_t(margin.x + gamepad.cell.x * cs, margin.y + (flipped ? (BH - gamepad.cell.y - 1) : gamepad.cell.y) * cs);
        pieceRenderer.render(gvm, base + cs / 2 + point_t(0, -6), held.piece, true);
      }
    }
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::mouseMoved(point_t p)
  {
    mouseMode = true;

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
  bool BoardGameRenderer<T, Renderer>::tryToPickupPieceAt(point_t coord)
  {
    if (game.canPickupPiece(coord))
    {
      auto& cell = game.get(coord);
      availableMoves = game.allowedMoves(cell, coord);
      if (!availableMoves.empty())
      {
        held = { true, coord, cell };
        cell = T();
        return true;
      }
    }
    return false;
  }

  template<typename T, typename Renderer>
  bool BoardGameRenderer<T, Renderer>::tryToDropPieceAt(point_t coord)
  {
    /* cancel move */
    if (coord == held.from)
    {
      held.present = false;
      game.get(held.from) = held.piece;
      held.piece = T();
      availableMoves.clear();
      return true;
    }
    else if (game.pieceMoved(held.piece, Move(held.from, coord)))
    {
      held.present = false;
      held.piece = T();
      availableMoves.clear();

      game.nextTurn();
      availableMovesForPlayer = game.allowedMoveSetForPlayer(game.currentPlayer());
      return true;
    }

    return false;
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::mouseButton(point_t p, MouseButton button, bool pressed)
  {
    if (pressed && button == MouseButton::Left && mouse.valid)
    {
      if (!held.present)
      {
        tryToPickupPieceAt(mouse.cell);
      }
      else
      {
        tryToDropPieceAt(mouse.cell);
      }
    }
  }

  template<typename T, typename Renderer>
  void BoardGameRenderer<T, Renderer>::gamepadButton(GamepadButton button, bool pressed)
  {
    mouseMode = false;

    if (!gamepad.valid)
    {
      gamepad.cell = { 0, 0 };
      gamepad.valid = true;
    }

    if (pressed)
    {

      switch (button)
      {

        case GamepadButton::DpadLeft:
          if (gamepad.cell.x > game.board().firstColumn())
            --gamepad.cell.x;
          break;
        case GamepadButton::DpadRight:
          if (gamepad.cell.x < game.board().lastColumn())
            ++gamepad.cell.x;
          break;
        case GamepadButton::DpadUp:
        case GamepadButton::DpadDown:
        {
          bool increasing = button == GamepadButton::DpadDown ^ !flipped;

          if (increasing && gamepad.cell.y > game.board().firstRow())
            --gamepad.cell.y;
          else if (!increasing && gamepad.cell.y < game.board().lastRow())
            ++gamepad.cell.y;

          break;
        }

        case GamepadButton::A:
        {
          if (!held.present)
            tryToPickupPieceAt(gamepad.cell);
          else
            tryToDropPieceAt(gamepad.cell);
          break;
        }

        case GamepadButton::B:
        {

          break;
        }
      }
    }

  }
}
