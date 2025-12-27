#ifndef FRAME_CONTEXT_H
#define FRAME_CONTEXT_H

#include "hex_tile_grid.h"
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
