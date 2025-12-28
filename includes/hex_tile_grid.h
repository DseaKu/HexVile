#ifndef HEX_TILE_GRid_H
#define HEX_TILE_GRid_H

#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "resource.h"
#include <atomic>
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

// --- Terrain Detail ---
struct TileDet {
  Vector2 tilePos;
  int taOffsetX;
};

// --- Map Tile ---
struct MapTile {
  tile::id id;
  TileDet det[conf::TERRAIN_DETAILS_MAX];
  rsrc::Object rsrc[conf::TERRAIN_RESOURCE_MAX];
};

/* Grid parts and relationships:
 * https://www.redblobgames.com/grids/parts/
 *
 * --- HEX TILE GRid ---
 *   |q  ,r-1|q+1,r-1|
 * ---------------------
|* q-1,r |q  ,r  |q+1, r|
 * -----------------------
 *   |q-1,r+1|q ,r+1 |
 */
class HexGrid {
private:
  std::vector<MapTile> tileData;
  int gridSize;

  // Stores currently visible tiles for rendering.
  std::vector<HexCoord> currentVisibleTiles;

  // Back buffer for visible tiles calculated asynchronously.
  std::vector<HexCoord> nextVisibleTiles;

  // Mutex to protect access to visiCache and visiCacheNext during swaps.
  std::mutex visiCacheMutex;

  // Manages the asynchronous calculation of visible tiles.
  std::future<void> visiCalcFuture;

  // Flag indicating if visiCacheNext has new data ready to be swapped.
  bool visiCacheReady;

  // Profiling
  std::atomic<double> calcVisTime;

  float tileGapX;
  float tileGapY;
  int animationFrame;
  int mapRadius;
  int tilesInUse;
  int tilesInTotal;
  Rectangle *camRect;
  Rectangle lastCamRect;
  Vector2 origin;

  // Gaussian distribution for terrain detail generation

  GFX_Manager *graphicsManager;

  // Lookup Tables
  static const std::vector<HexCoord> DIRECTIONS;
  static const std::vector<tile::id> WALKABLE_TILES;

  // --- Internal Helpers ---
  HexCoord HexRound(FractionalHex h) const;
  const MapTile &GetTile(HexCoord h) const;
  MapTile &GetTile(HexCoord h);
  TileDet GetRandomTerainDetail(tile::id tileID);
  rsrc::Object GetRandomTerainResource(tile::id tileID);
  void CalcRenderRect();
  void CalcVisibleTiles();
  void UpdateTileVisibility(float totalTime);

  // --- Core Lifecycle ---
  void UpdateTilesProperties();

  // --- Render ---
  void LoadTileGFX(Rectangle destRec, int x, int y);
  void LoadDetailGFX(Rectangle destRec, const TileDet d, tile::id tileID);
  void LoadResourceGFX(Rectangle destRec, const rsrc::Object r,
                       tile::id tileID);

public:
  HexGrid();

  // --- Core Lifecycle ---
  void InitGrid(float radius);
  void Update(const Camera2D &camera, float totalTime);
  void Shutdown();

  // --- Setters ---
  void SetGFX_Manager(GFX_Manager *graphicsManager);
  void SetCamRectPointer(Rectangle *camRect);
  bool SetTile(HexCoord h, tile::id tileID);

  // --- Getters & State Checks ---
  int GetTilesInUse() const;
  int GetTilesInTotal() const;
  int GetTilesVisible() const;
  int GetMapRadius() const;
  bool IsInBounds(HexCoord h) const;
  bool HasTile(HexCoord h) const;
  bool IsWalkable(HexCoord h) const;
  bool CheckSurrounded(HexCoord target) const;

  bool RemoveResource(HexCoord h, int rsrcID);
  bool DamageResource(HexCoord h, int rsrcID, int damage);

  // --- Conversions & Helpers ---
  HexCoord PointToHexCoord(Vector2 point) const;
  Vector2 HexCoordToPoint(HexCoord h) const;
  Vector2 CoordToPoint(int q, int r) const;
  MapTile *HexCoordToTile(HexCoord h);
  const MapTile *HexCoordToTile(HexCoord h) const;
  MapTile *PointToTile(Vector2 point);
  const MapTile *PointToTile(Vector2 point) const;
  tile::id PointToType(Vector2 point) const;
  tile::id HexCoordToType(HexCoord h) const;
  const char *TileToString(tile::id tileID) const;
  HexCoord GetNeighbor(HexCoord h, int directionIndex) const;
  double GetVisCalcTime() const;

  bool CheckObstacleCollision(Vector2 worldPos, float radius);

  // --- Direct Drawing (for debugging/special cases) ---
  void DrawTile(HexCoord h, int TA_X, int TA_Y, drawMask::id layerID);
};

#endif // HEX_TILE_GRid_H
