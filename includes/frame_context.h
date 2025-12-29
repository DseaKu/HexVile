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

struct Location {
  // Player
  Vector2 player;

  // Mouse
  Vector2 mouseScreen;
  Vector2 mouseWorld;

  // Tile
  HexCoord hoveredTileHexCoords;
  Vector2 hoveredTilePoint;
  MapTile *hoveredTile;

  // Resource
  Vector2 hoveredRsrcPoint;
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
