#ifndef HEX_TILE_GRID_H
#define HEX_TILE_GRID_H

#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "texture_handler.h"
#include <future>
#include <mutex>
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
struct TerrainDetail {
  u8 x;
  u8 y;
  int detail;
};

// --- Map Tile ---
struct MapTile {
  int version;
  TileID type;
  TerrainDetail detail[Conf::TERRAIN_DETAIL_MAX];
};

// --- Visibilty Cache ---
struct VisibiltyData {
  u32 r;
  u32 q;
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
  std::vector<VisibiltyData>
      visiCache; // Stores currently visible tiles for rendering.
  std::vector<VisibiltyData>
      visiCacheNext; // Back buffer for visible tiles calculated asynchronously.
  std::mutex visiCacheMutex; // Mutex to protect access to visiCache and
                             // visiCacheNext during swaps.
  std::future<void>
      visiCalcFuture;  // Manages the asynchronous calculation of visible tiles.
  bool visiCacheReady; // Flag indicating if visiCacheNext has new data ready to
                       // be swapped.
  float tileGapX;
  float tileGapY;
  int animationFrame;
  int mapRadius;
  int tilesInUse;
  int tilesInTotal;
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
  void AddGrassDetails(int amount);

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
  void UpdateGrid();
  bool IsWalkable(HexCoord h);

  // --- Rendering ---
  void Draw(const Camera2D &camera);
  void DrawDetails(Rectangle sourceRect, Rectangle destRect);

  // --- Get ---
  int GetTilesInUse();
  int GetTilesInTotal();
  int GetTilesVisible();
  int GetMapRadius();
  HexCoord GetNeighbor(HexCoord h, int directionIndex);

  // --- Set ---
  void SetCamRectPointer(Rectangle *camRect);
};

#endif // HEX_TILE_GRID_H
