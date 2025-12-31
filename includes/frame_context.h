#ifndef FRAME_CONTEXT_H
#define FRAME_CONTEXT_H

#include "hex_tile_grid.h"
#include "raylib.h"
namespace frame {

struct InputCommands {
  // Tool bar slots
  bool slot0;
  bool slot1;
  bool slot2;
  bool slot3;
  bool slot4;
  bool slot5;
  bool slot6;
  bool slot7;
  bool slot8;
  bool slot9;

  // Movement
  bool up;
  bool down;
  bool left;
  bool right;

  // Menu
  bool toggleInventory;
};

struct MouseInput {
  bool left;
  bool right;
};

struct Input {
  MouseInput mouseClick;
  MouseInput mouseDown;
  InputCommands commands;
};

struct WorldPos {
  Vector2 player;
  Vector2 mouse;
  Vector2 hoveredRsrcPoint;
  HexCoord hoveredTileHexCoords;
  Vector2 hoveredTilePoint;
  MapTile *hoveredTile;
};

struct ScreenPos {
  Vector2 mouse;
  Vector2 center;
};

struct Context {
  WorldPos worldPos;
  ScreenPos screenPos;
  Input inputs;
  mouseMask::id mouseMask;
  int selToolBarSlot;

  // Screen
  float screenWidth;
  float screenHeight;

  float deltaTime;
};
} // namespace frame

#endif // !FRAME_CONTEXT_H
