#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"
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
  TileID type;
  bool isDirty;
  bool isVisble;
};

// --- HEX TILE GRID ---
class HexGrid {
private:
  std::map<HexCoord, MapTile> HexTiles;
  float tileGapX;
  float tileGapY;
  int mapRadius;
  Vector2 origin;
  TextureHandler *textureHandler;

  // Lookup Tables
  static const std::vector<HexCoord> DIRECTIONS;
  static const std::vector<std::string> DIR_LABELS;

  // Internal Math Helper
  HexCoord HexRound(FractionalHex h) const;

public:
  HexGrid();

  void InitGrid(float radius);
  void GetTextureHandler(TextureHandler *textureHandler);

  // Coordinate Conversions
  Vector2 HexCoordToPoint(HexCoord h) const;
  HexCoord PointToHexCoord(Vector2 point) const;

  // Logic
  HexCoord GetNeighbor(HexCoord h, int directionIndex) const;
  bool HasTile(HexCoord h) const;
  void ToggleTile(HexCoord h);
  bool CheckSurrounded(HexCoord target) const;

  // Rendering
  void Draw(const Camera2D &camera);
};

#endif // HEX_TILE_GRID_H
