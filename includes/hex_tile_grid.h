#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"
#include <vector>

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

// --- STRUCTS ---
struct FractionalHex {
  double q, r, s;
};

struct MapTile {
  TileID type;
  bool isDirty;
  bool isVisble;
};

/* Grid parts and relationships: https://www.redblobgames.com/grids/parts/
 *
 *
 * --- HEX TILE GRID ---
 *   |q  ,r-1|q+1,r-1|
 * ---------------------
|* q-1,r |q  ,r  |q+1, r|
 * -----------------------
 *   |q-1,r+1|q ,r+1 |
 */
class HexGrid {
private:
  std::vector<std::vector<MapTile>> tiles;
  float tileGapX;
  float tileGapY;
  int mapRadius;
  int tilesInUse;
  Vector2 origin;
  TextureHandler *textureHandler;

  // Lookup Tables
  static const std::vector<HexCoord> DIRECTIONS;
  static const std::vector<TileID> WALKABLE_TILES;

  // Internal Math Helper
  HexCoord HexRound(FractionalHex h);

public:
  HexGrid();

  void InitGrid(float radius);
  void SetTextureHandler(TextureHandler *textureHandler);

  // Coordinate Conversions
  Vector2 HexCoordToPoint(HexCoord h);
  MapTile HexCoordToTile(HexCoord h);
  HexCoord PointToHexCoord(Vector2 point);
  MapTile PointToTile(Vector2 point);
  TileID PointToType(Vector2 point);
  const char *TileToString(TileID type);

  // Logic
  bool IsInBounds(HexCoord h);
  HexCoord GetNeighbor(HexCoord h, int directionIndex);
  bool HasTile(HexCoord h);
  void SetTile(HexCoord h, TileID ID);
  void ToggleTile(HexCoord h);
  bool CheckSurrounded(HexCoord target);

  // Rendering
  void Draw(const Camera2D &camera);
  bool IsWalkable(HexCoord h);

  // Set/Get
  int getTilesInUse();
  int getMapRadius();
};

#endif // HEX_TILE_GRID_H
