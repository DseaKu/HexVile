#include "defines.h"
#include "game.h"
#include "raylib.h"

int main(void) {
  InitWindow(Conf::SCREEN_WIDTH, Conf::SCREEN_HEIGHT, Conf::WINDOW_TITLE);
  SetTargetFPS(Conf::MAX_FPS);
  Game game;
  game.GameLoop();
  game.Unload();
  CloseWindow();
  return 0;
}
