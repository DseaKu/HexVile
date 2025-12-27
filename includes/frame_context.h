#ifndef FRAME_CONTEXT_H
#define FRAME_CONTEXT_H

#include "hex_tile_grid.h"
namespace frame {

struct KeyboardInput {
  bool One;
  bool Two;
  bool Three;
  bool Four;
  bool Five;
  bool Six;
  bool Seven;
  bool Eight;
  bool Nine;
  bool Zero;
  bool Up;
  bool Down;
  bool Left;
  bool Right;
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
  Vector2 mouseScreen;
  Vector2 mouseWorld;
  MapTile *hoveredTile;
};

struct Context {
  Location pos;
  Input inputs;
  mouseMask::id mouseMask;
  int selToolBarSlot;

  // Screen
  int screenWidth;
  int screenHeight;

  float deltaTime;
};
} // namespace frame

#endif // !FRAME_CONTEXT_H
