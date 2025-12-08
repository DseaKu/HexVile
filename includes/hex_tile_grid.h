#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "enums.h"
#include "raylib.h"
#include <map>
#include <string>
#include <vector>

// --- STRUCTS & ENUMS ---

struct FractionalHex {
  double q, r, s;
};

// --- HEXAGON ---
class HexCoord {
public:
  int q;
  int r;

  // Constructors
  HexCoord();
  HexCoord(int q, int r);

  // Logic Methods
  HexCoord Add(const HexCoord &other) const;
  bool Equals(const HexCoord &other) const;

  // Operators
  HexCoord operator+(const HexCoord &other) const;
  bool operator==(const HexCoord &other) const;
  bool operator!=(const HexCoord &other) const;
  bool operator<(const HexCoord &other) const;
};

struct MapTile {
  HexCoord coord;
  TileType type;
};

// --- HEX TILE GRID ---
class HexGrid {
private:
  std::map<HexCoord, MapTile> HexTiles;
  float hexRadius;
  int mapRadius;
  Vector2 origin;

  // Lookup Tables
  const std::vector<HexCoord> DIRECTIONS;
  const std::vector<std::string> DIR_LABELS;

  // Internal Math Helper
  HexCoord HexRound(FractionalHex h) const;

public:
  HexGrid(float radius, int mapSize, Vector2 CenterPos);

  void InitGrid();

  // Coordinate Conversions
  Vector2 HexCoordToPoint(HexCoord h) const;
  HexCoord PointToHexCoord(Vector2 point) const;

  // Logic
  HexCoord GetNeighbor(HexCoord h, int directionIndex) const;
  bool HasTile(HexCoord h) const;
  void ToggleTile(HexCoord h);
  bool CheckSurrounded(HexCoord target) const;

  // Rendering
  void Draw();
  void DrawDebugOverlay(Vector2 mousePos);
};

#endif // HEX_TILE_GRID_H
