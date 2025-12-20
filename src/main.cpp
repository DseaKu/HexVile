#include "defines.h"
#include "game.h"
#include "raylib.h"

int main(void) {
  // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT |
  // FLAG_WINDOW_HIGHDPI);
  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(conf::SCREEN_WIDTH, conf::SCREEN_HEIGHT, conf::WINDOW_TITLE);
  SetTargetFPS(conf::MAX_FPS);
  Game game;
  game.GameLoop();
  game.Unload();
  CloseWindow();
  return 0;
}
