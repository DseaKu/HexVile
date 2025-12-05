#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "raylib.h"
#include <map>
#include <string>
#include <vector>

// --- STRUCTS & ENUMS ---

struct FractionalHex {
  double q, r, s;
};

enum TileType { EMPTY, WALL };

// --- HEX CLASS ---
class HexTile {
public:
  int q;
  int r;

  // Constructors
  HexTile();
  HexTile(int q, int r);

  // Logic Methods
  HexTile Add(const HexTile &other) const;
  bool Equals(const HexTile &other) const;

  // Operators
  HexTile operator+(const HexTile &other) const;
  bool operator==(const HexTile &other) const;
  bool operator!=(const HexTile &other) const;
  bool operator<(const HexTile &other) const;
};

struct Tile {
  HexTile coord;
  TileType type;
};

// --- HEX GRID CLASS ---
class HexGrid {
private:
  std::map<HexTile, Tile> tiles;
  float hexRadius;
  int mapRadius;
  Vector2 origin;

  // Lookup Tables
  const std::vector<HexTile> DIRECTIONS;
  const std::vector<std::string> DIR_LABELS;

  // Internal Math Helper
  HexTile HexRound(FractionalHex h) const;

public:
  HexGrid(float radius, int mapSize, Vector2 CenterPos);

  void InitGrid();

  // Coordinate Conversions
  Vector2 HexTileToPixel(HexTile h) const;
  HexTile PixelToHexTile(Vector2 point) const;

  // Logic
  HexTile GetNeighbor(HexTile h, int directionIndex) const;
  bool HasTile(HexTile h) const;
  void ToggleTile(HexTile h);
  bool CheckSurrounded(HexTile target) const;

  // Rendering
  void Draw();
  void DrawDebugOverlay(Vector2 mousePos);
};

#endif // HEX_TILE_GRID_H
