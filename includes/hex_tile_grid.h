#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "defines.h"
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

struct FractionalHex {
  double q, r, s;
};

// --- Terrain Detail --- TD = Terrain Detail
typedef u16 TerrainDetail;
#define TD_SHIFT_TYPE 0u
#define TD_SHIFT_X 8u
#define TD_SHIFT_Y 12u

#define TD_MASK_TYPE 0x8
#define TD_MASK_X 0x4
#define TD_MASK_Y 0x4

// --- Map Tile ---
struct MapTile {
  int version;
  TileID type;
  bool isVisble;
  TerrainDetail detail[Conf::TERRAIN_DETAIL_MAX];
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
  std::vector<std::vector<MapTile>> tileData;
  float tileGapX;
  float tileGapY;
  int animationFrame;
  int mapRadius;
  int tilesInUse;
  int tilesInTotal;
  int calcVisibleTilesCounter;
  Rectangle *camRect;
  Vector2 origin;
  TextureHandler *textureHandler;

  // Lookup Tables
  static const std::vector<HexCoord> DIRECTIONS;
  static const std::vector<TileID> WALKABLE_TILES;

  // Internal Math Helper
  HexCoord HexRound(FractionalHex h);

  // --- Logic ---
  void CalcVisibleTiles();

public:
  HexGrid();

  void InitGrid(float radius);
  void SetTextureHandler(TextureHandler *textureHandler);

  // --- Conversions ---
  Vector2 HexCoordToPoint(HexCoord h);
  Vector2 CoordToPoint(int r, int q);
  MapTile HexCoordToTile(HexCoord h);
  TileID HexCoordToType(HexCoord h);
  HexCoord PointToHexCoord(Vector2 point);
  MapTile PointToTile(Vector2 point);
  TileID PointToType(Vector2 point);
  const char *TileToString(TileID type);

  // --- Logic ---
  bool IsInBounds(HexCoord h);
  bool HasTile(HexCoord h);
  bool SetTile(HexCoord h, TileID ID);
  void ToggleTile(HexCoord h);
  bool CheckSurrounded(HexCoord target);

  // --- Rendering ---
  void Draw(const Camera2D &camera);
  bool IsWalkable(HexCoord h);

  // --- Get ---
  int GetTilesInUse();
  int GetTilesInTotal();
  int GetMapRadius();
  HexCoord GetNeighbor(HexCoord h, int directionIndex);

  // --- Set ---
  void SetCamRectPointer(Rectangle *camRect);
};

#endif // HEX_TILE_GRID_H
