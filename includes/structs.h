#ifndef STRUCTS_H
#define STRUCTS_H

#include "hex_tile_grid.h"
#include "raylib.h"
#include <string>
#include <vector>

struct DebugData {
  std::string section;
  std::vector<std::string> subSection;
};

struct RenderState {
  Camera2D camera;

  // Resources / Stats
  int tilesTotal;
  int tilesUsed;
  int tilesVisible;
  int mapRadius;
  double visCalcTime;

  // Mouse Hover
  HexCoord mouseTileCoord;
  tile::id mouseTileType;

  // Player
  Vector2 playerPos;
  HexCoord playerTileCoord;
  tile::id playerTileID;
  std::string playerStateStr;
  std::string playerDirStr;
  int playerFrame;
  float playerSpeed;

  // ToolBar
  std::string selectedItemType;
  int selectedToolBarSlot;
};

#endif // !STRUCTS_H
