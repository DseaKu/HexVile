#ifndef FRAME_CONTEXT_H
#define FRAME_CONTEXT_H

#include "hex_tile_grid.h"
#include "raylib.h"
namespace frame {

struct KeyboardInput {
  bool one;
  bool two;
  bool three;
  bool four;
  bool five;
  bool six;
  bool seven;
  bool eight;
  bool nine;
  bool zero;
  bool up;
  bool down;
  bool left;
  bool right;
};

struct MouseInput {
  bool left;
  bool right;
};

struct Input {
  MouseInput mouseClick;
  MouseInput mouseDown;
  KeyboardInput keyPress;
};

struct Location {
  Vector2 player;

  Vector2 mouseScreen;
  Vector2 mouseWorld;

  HexCoord hoveredTileHexCoords;
  Vector2 hoveredTilePoint;

  Vector2 hoveredRsrcPoint;
};

struct Context {
  Location pos;
  Input inputs;
  mouseMask::id mouseMask;
  int selToolBarSlot;
  MapTile *hoveredTile;

  // Screen
  int screenWidth;
  int screenHeight;

  float deltaTime;
};
} // namespace frame

#endif // !FRAME_CONTEXT_H
