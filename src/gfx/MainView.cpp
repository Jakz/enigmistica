#pragma once

#include "MainView.h"
#include "ViewManager.h"

using namespace ui;

u32* buffer = nullptr;
u64 offset = 0;

MainView::MainView(ViewManager* gvm) : gvm(gvm)
{
  mouse = { -1, -1 };
}

void MainView::render()
{
  auto r = gvm->renderer();

  gvm->clear({ 255, 255, 255 });

  int w = 22, h = 12;
  int32_t s = 12;

  for (int32_t x = 0; x < w + 1; ++x)
    gvm->line(1 + x * s, 1, 1 + x * s, 1 + h * s, { 0, 0, 0 });

  for (int32_t y = 0; y < h + 1; ++y)
    gvm->line(1, 1 + y * s, 1 + w * s, 1 + y * s, { 0, 0, 0 });

  for (int32_t y = 0; y < h; ++y)
    for (int32_t x = 0; x < w; ++x)
      if ((y + x) % 7 == 0 || (y % 5 == 0) || (x % 3 == 0))
        gvm->fillRect(1 + x * s + 2, 1 + y * s + 2, s - 3, s - 3, { 0, 0, 0 });

}

/*void MainView::render()
{
  auto r = gvm->renderer();

  SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
  SDL_RenderClear(r);

  const int32_t spacing = 10;
  int32_t x = 6, y = 6;
  const int32_t lines = (HEIGHT - y) / spacing;

  if (!buffer)
  {
    FILE* in = fopen("F:/Misc/Roms/Nintendo - NES/Adventure Island 2.nes", "rb");
    buffer = new u32[lines * 4];
    fread(buffer, sizeof(u32), lines * 4, in);
    fclose(in);
  }

  for (int32_t i = 0; i < lines; ++i)
  {
    static char sbuffer[256];
    snprintf(sbuffer, 256, "%04x %08x %08x %08x", i * 4, buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
    gvm->text(sbuffer, x, y + i * spacing, { 255, 255, 255 }, ui::TextAlign::LEFT, 1.0f);
  }

  if (mouse.x >= 0 && mouse.x < WIDTH && mouse.y >= 0 && mouse.y < HEIGHT)
  {
    gvm->fillRect(mouse.x - 1, mouse.y - 1, 3, 3, { 255, 0, 0, 0 });
  }

  gvm->drawRect(0, 0, 34, HEIGHT, { 160, 160, 160, 0 });
}*/

void MainView::handleKeyboardEvent(const SDL_Event& event)
{
  if (event.type == SDL_KEYDOWN)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE: gvm->exit(); break;
    }
  }
}

void MainView::handleMouseEvent(const SDL_Event& event)
{
  if (event.type == SDL_MOUSEMOTION)
  {
    mouse.x = event.motion.x;
    mouse.y = event.motion.y;
  }
}