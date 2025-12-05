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

// --- HEXAGON ---
class Hexagon {
public:
  int q;
  int r;

  // Constructors
  Hexagon();
  Hexagon(int q, int r);

  // Logic Methods
  Hexagon Add(const Hexagon &other) const;
  bool Equals(const Hexagon &other) const;

  // Operators
  Hexagon operator+(const Hexagon &other) const;
  bool operator==(const Hexagon &other) const;
  bool operator!=(const Hexagon &other) const;
  bool operator<(const Hexagon &other) const;
};

struct Tile {
  Hexagon coord;
  TileType type;
};

// --- HEX TILE GRID ---
class HexGrid {
private:
  std::map<Hexagon, Tile> HexTiles;
  float hexRadius;
  int mapRadius;
  Vector2 origin;

  // Lookup Tables
  const std::vector<Hexagon> DIRECTIONS;
  const std::vector<std::string> DIR_LABELS;

  // Internal Math Helper
  Hexagon HexRound(FractionalHex h) const;

public:
  HexGrid(float radius, int mapSize, Vector2 CenterPos);

  void InitGrid();

  // Coordinate Conversions
  Vector2 HexagonToPoint(Hexagon h) const;
  Hexagon PointToHexagon(Vector2 point) const;

  // Logic
  Hexagon GetNeighbor(Hexagon h, int directionIndex) const;
  bool HasTile(Hexagon h) const;
  void ToggleTile(Hexagon h);
  bool CheckSurrounded(Hexagon target) const;

  // Rendering
  void Draw();
  void DrawDebugOverlay(Vector2 mousePos);
};

#endif // HEX_TILE_GRID_H
