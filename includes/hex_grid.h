#ifndef HEX_GRID_H
#define HEX_GRID_H

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
class Hex {
public:
  int q;
  int r;

  // Constructors
  Hex();
  Hex(int q, int r);

  // Logic Methods
  Hex Add(const Hex &other) const;
  bool Equals(const Hex &other) const;

  // Operators
  Hex operator+(const Hex &other) const;
  bool operator==(const Hex &other) const;
  bool operator!=(const Hex &other) const;
  bool operator<(const Hex &other) const; // Required for std::map
};

struct Tile {
  Hex coord;
  TileType type;
};

// --- HEX GRID CLASS ---
class HexGrid {
private:
  std::map<Hex, Tile> tiles;
  float hexRadius;
  int mapRadius;
  Vector2 origin;

  // Lookup Tables
  const std::vector<Hex> DIRECTIONS;
  const std::vector<std::string> DIR_LABELS;

  // Internal Math Helper
  Hex HexRound(FractionalHex h) const;

public:
  HexGrid(float radius, int mapSize, Vector2 centerPos);

  void InitGrid();

  // Coordinate Conversions
  Vector2 HexToPixel(Hex h) const;
  Hex PixelToHex(Vector2 point) const;

  // Logic
  Hex GetNeighbor(Hex h, int directionIndex) const;
  bool HasTile(Hex h) const;
  void ToggleTile(Hex h);
  bool CheckSurrounded(Hex target) const;

  // Rendering
  void Draw();
  void DrawDebugOverlay(Vector2 mousePos);
};

#endif // HEX_GRID_H
