#include "hex_tile_grid.h"
#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "resource.h"
#include "texture.h"
#include "tile_details.h"
#include <cmath>
#include <vector>

const std::vector<HexCoord> HexGrid::DIRECTIONS = {
    HexCoord(1, 0),  HexCoord(0, 1),  HexCoord(-1, 1),
    HexCoord(-1, 0), HexCoord(0, -1), HexCoord(1, -1)};

// ============= Hex Coordinates ====================
HexCoord::HexCoord() : q(0), r(0) {}
HexCoord::HexCoord(int q, int r) : q(q), r(r) {}

HexCoord HexCoord::Add(const HexCoord &other) const {
  return HexCoord(q + other.q, r + other.r);
}

bool HexCoord::Equals(const HexCoord &other) const {
  return q == other.q && r == other.r;
}

HexCoord HexCoord::operator+(const HexCoord &other) const { return Add(other); }

bool HexCoord::operator==(const HexCoord &other) const { return Equals(other); }

bool HexCoord::operator!=(const HexCoord &other) const {
  return !Equals(other);
}

bool HexCoord::operator<(const HexCoord &other) const {
  if (q != other.q)
    return q < other.q;
  return r < other.r;
}

// ============= Hex Grid ===================
HexGrid::HexGrid() {

  animationFrame = 0;
  tileGapX = conf::TILE_SPACING_X;
  tileGapY = conf::TILE_SPACING_Y;
  origin = conf::SCREEN_CENTER;
  mapRadius = conf::MAP_RADIUS;
  gridSize = 0;
  tilesInUse = 0;
  tilesInTotal = 0;
  camRect = nullptr;
  lastCamRect = {0, 0, 0, 0};
  visiCacheReady = false;

  size_t estimated_hits = conf::ESTIMATED_VISIBLE_TILES;
  currentVisibleTiles.reserve(estimated_hits);
  nextVisibleTiles.reserve(estimated_hits);

  calcVisTime = 0.0;
}

void HexGrid::InitGrid(float radius) {

  gridSize = mapRadius * 2 + 1;
  tileData.resize(gridSize * gridSize);

  // Init tile data
  for (int r = -mapRadius; r <= mapRadius; r++) {
    for (int q = -mapRadius; q <= mapRadius; q++) {
      HexCoord h(q, r);
      int gridR = r + mapRadius;
      int gridQ = q + mapRadius;
      this->tilesInTotal++;

      Vector2 worldPos = CoordToPoint(q, r);

      if (abs(q) + abs(r) + abs(-q - r) <= mapRadius * 2) {

        MapTile initTile = {.id = tile::GRASS};
        initTile.posWorld = worldPos;

        for (TileDet &d : initTile.det) {
          d.taOffsetX = conf::UNINITIALIZED;
        }

        rsrc::Object &rsrcObj = initTile.rsrc;
        rsrcObj.id = rsrc::UNINITIALIZED;

        tileData[gridR * gridSize + gridQ] = initTile;
        this->tilesInUse++;

      } else {
        tileData[gridR * gridSize + gridQ] =
            (MapTile){.id = tile::NULL_ID, .posWorld = worldPos};
      }
    }
  }
  CalcVisibleTiles();
}

TileDet HexGrid::GetRandomTerainDetail(tile::id id) {
  float x = GetRandomValue(-tex::size::QUATER_TILE, tex::size::QUATER_TILE);
  float y = GetRandomValue(-tex::size::QUATER_TILE, tex::size::QUATER_TILE);

  auto spawnData = spawn_data_det::detLut.at(id);

  int totalWeight = conf::TOTAL_WEIGHT_DET;
  int taOffsetX = conf::SKIP_RENDER;
  int index = GetRandomValue(0, spawnData.size() - 1);

  int randNum = GetRandomValue(0, totalWeight);
  if (randNum <= spawnData[index]) {
    taOffsetX = index;
  }

  return TileDet{.tilePos = Vector2{x, y}, .taOffsetX = taOffsetX};
}

rsrc::Object HexGrid::GetRandomTerainResource(tile::id id,
                                              Vector2 tileWorldPos) {

  auto spawnData = rsrc::TILE_LUT.at(id);

  float x = GetRandomValue(-conf::SPAWN_RSRC_SPREAD, conf::SPAWN_RSRC_SPREAD);
  float y = GetRandomValue(-conf::SPAWN_RSRC_SPREAD, conf::SPAWN_RSRC_SPREAD);

  spawnData.worldPos = {tileWorldPos.x + x, tileWorldPos.y + y};

  int totalWeight = conf::TOTAL_WEIGHT_RSRC;
  rsrc::Object rsrc = rsrc::OBJECT_NULL;
  int index = GetRandomValue(0, totalWeight);

  int randNum = GetRandomValue(0, totalWeight);
  if (randNum <= spawnData.spawn_chance) {
    rsrc = spawnData;
  }

  return rsrc;
}

void HexGrid::SetGFX_Manager(GFX_Manager *graphicsManager) {
  this->graphicsManager = graphicsManager;
}

bool HexGrid::IsInBounds(HexCoord h) const {
  int s = -h.q - h.r;
  return abs(h.q) <= mapRadius && abs(h.r) <= mapRadius && abs(s) <= mapRadius;
}

bool HexGrid::HasTile(HexCoord h) const {
  if (!IsInBounds(h)) {
    return false;
  }
  return GetTile(h).id != tile::NULL_ID;
}

bool HexGrid::IsWalkable(HexCoord h) const {
  if (!HasTile(h)) {
    return false;
  }
  const MapTile *tile = HexCoordToTile(h);
  if (!tile)
    return false;

  tile::id type = tile->id;
  for (int i = 0; i < conf::WALKABLE_TILE_IDS.size(); i++) {
    if (type == conf::WALKABLE_TILE_IDS[i]) {
      return true;
    }
  }
  return false;
}

const MapTile &HexGrid::GetTile(HexCoord h) const {
  return tileData[(h.r + mapRadius) * gridSize + (h.q + mapRadius)];
}

MapTile &HexGrid::GetTile(HexCoord h) {
  return tileData[(h.r + mapRadius) * gridSize + (h.q + mapRadius)];
}

// ============= Conversion =================
HexCoord HexGrid::HexRound(FractionalHex h) const {
  int q = std::round(h.q);
  int r = std::round(h.r);
  int s = std::round(h.s);
  double q_diff = std::abs(q - h.q);
  double r_diff = std::abs(r - h.r);
  double s_diff = std::abs(s - h.s);
  if (q_diff > r_diff && q_diff > s_diff)
    q = -r - s;
  else if (r_diff > s_diff)
    r = -q - s;
  return HexCoord(q, r);
}

Vector2 HexGrid::HexCoordToPoint(HexCoord h) const {
  return CoordToPoint(h.q, h.r);
}

Vector2 HexGrid::CoordToPoint(int q, int r) const {
  float x = tileGapX * (std::sqrt(3.0f) * q + std::sqrt(3.0f) / 2.0f * r);
  float y = tileGapY * (3.0f / 2.0f * r);
  return {x + origin.x, y + origin.y};
}

tile::id HexGrid::PointToType(Vector2 point) const {
  const MapTile *tile = PointToTile(point);
  return tile ? tile->id : tile::NULL_ID;
}

tile::id HexGrid::HexCoordToType(HexCoord h) const {
  const MapTile *m = HexCoordToTile(h);
  return m ? m->id : tile::NULL_ID;
}

MapTile *HexGrid::HexCoordToTile(HexCoord h) {
  if (!IsInBounds(h)) {
    return nullptr;
  }
  return &GetTile(h);
}

const MapTile *HexGrid::HexCoordToTile(HexCoord h) const {
  if (!IsInBounds(h)) {
    return nullptr;
  }
  return &GetTile(h);
}

MapTile *HexGrid::PointToTile(Vector2 point) {
  HexCoord h = PointToHexCoord(point);
  return HexCoordToTile(h);
}

const MapTile *HexGrid::PointToTile(Vector2 point) const {
  HexCoord h = PointToHexCoord(point);
  return HexCoordToTile(h);
}

HexCoord HexGrid::PointToHexCoord(Vector2 point) const {
  float pt_x = (point.x - origin.x) / tileGapX;
  float pt_y = (point.y - origin.y) / tileGapY;
  double q = (std::sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y);
  double r = (2.0 / 3.0 * pt_y);
  return HexRound({q, r, -q - r});
}

const char *HexGrid::TileToString(tile::id id) const {
  switch (id) {
  case tile::NULL_ID:
    return "NULL";
  case tile::GRASS:
    return "Grass";
  case tile::WATER:
    return "Water";
  case tile::DIRT:
    return "Dirt";
  default:
    return "Undefined";
  }
}

// ============= Logic =======================

void HexGrid::CalcVisibleTiles() {
  auto start = std::chrono::high_resolution_clock::now();
  if (camRect == nullptr) {
    return;
  }
  // Define the rendering view rectangle, expanded by an offset for culling.
  Rectangle renderView = {
      .x = camRect->x - conf::RENDER_VIEW_CULLING_MARGIN,
      .y = camRect->y - conf::RENDER_VIEW_CULLING_MARGIN,
      .width = camRect->width + conf::RENDER_VIEW_CULLING_EXPANSION,
      .height = camRect->height + conf::RENDER_VIEW_CULLING_EXPANSION};

  int gridSize = mapRadius * 2 + 1;

  std::vector<HexCoord> newVisiCache;
  newVisiCache.reserve(conf::ESTIMATED_VISIBLE_TILES); // Pre-allocate memory
                                                       // for efficiency.

  // Iterate over all tiles in the grid.
  for (u16 r = 0; r < gridSize; r++) {
    for (u16 q = 0; q < gridSize; q++) {

      if (tileData[r * gridSize + q].id == tile::NULL_ID) {
        continue;
      }
      // Convert grid coordinates to HexCoord and then to screen coordinates.
      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= conf::TILE_RESOLUTION_HALF;
      pos.y -= conf::TILE_RESOLUTION_HALF;
      Rectangle dest_rect = {pos.x, pos.y, tex::size::TILE, tex::size::TILE};
      // Check if the tile's bounding box intersects with the render view.
      if (CheckCollisionRecs(renderView, dest_rect)) {
        newVisiCache.push_back(h);
      }
    }
  }
  // Lock the mutex to safely swap the newly calculated visible tiles into the
  // nextVisibleTiles. std::lock_guard ensures the mutex is unlocked when
  // exiting this scope.
  std::lock_guard<std::mutex> lock(visiCacheMutex);
  nextVisibleTiles =
      std::move(newVisiCache); // Move the new data to the back buffer.
  visiCacheReady = true; // Signal that new data is ready for the main thread.

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  calcVisTime = elapsed.count();
}

bool HexGrid::CheckSurrounded(HexCoord target) const {
  if (!IsInBounds(target)) {
    return false;
  }

  int neighborCount = 0;
  int wallCount = 0;

  for (int i = 0; i < 6; i++) {
    HexCoord n = GetNeighbor(target, i);
    if (IsInBounds(n)) {
      neighborCount++;
      if (GetTile(n).id == tile::NULL_ID) {
        wallCount++;
      }
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

bool HexGrid::RemoveResource(HexCoord h, int id) {
  if (!HasTile(h)) {
    return false;
  }
  MapTile &tile = GetTile(h);
  rsrc::Object &rsrc = tile.rsrc;
  if (rsrc.id == id) {
    rsrc.id = rsrc::ID_NULL;
    return true;
  }
  return false;
}

bool HexGrid::DamageResource(HexCoord h, int id, int damage) {
  if (!HasTile(h)) {
    return false;
  }
  MapTile &tile = GetTile(h);
  rsrc::Object &rsrc = tile.rsrc;
  if (rsrc.id == id) {
    rsrc.hp -= damage;
    rsrc.flashTimer = 0.15f; // Flash for 150ms

    if (rsrc.hp <= 0) {
      rsrc.id = rsrc::ID_NULL;
      return true;
    }
    return false;
  }
  return false;
}

void HexGrid::DrawTile(HexCoord h, int TA_X, int TA_Y, drawMask::id layerID) {
  if (!HasTile(h)) {
    return;
  }
  Vector2 pos = HexCoordToPoint(h);
  pos.x -= conf::TILE_RESOLUTION_HALF;
  pos.y -= conf::TILE_RESOLUTION_HALF;

  tex::Opts opts;
  opts.origin = {0.0f, 0.0f};
  graphicsManager->LoadTextureToBackbuffer(layerID, {TA_X, TA_Y}, pos, opts);
}

void HexGrid::UpdateTileVisibility(float totalTime) {
  // Check if the asynchronous calculation of visible tiles is complete
  if (visiCacheReady) {

    // Lock the mutex to safely swap the current rendering cache
    std::lock_guard<std::mutex> lock(visiCacheMutex);
    currentVisibleTiles.swap(nextVisibleTiles); // Atomically swap the buffers.
    nextVisibleTiles.clear(); // Clear the now-empty back buffer.
    visiCacheReady = false;   // Reset the flag.
  }

  // Check if a previous asynchronous calculation is still running.
  // If not valid (first run) or finished, launch a new one.
  if (!visiCalcFuture.valid() || visiCalcFuture.wait_for(std::chrono::seconds(
                                     0)) == std::future_status::ready) {

    bool cameraMoved = false;
    if (camRect != nullptr) {
      if (camRect->x != lastCamRect.x || camRect->y != lastCamRect.y ||
          camRect->width != lastCamRect.width ||
          camRect->height != lastCamRect.height) {
        cameraMoved = true;
      }
    }

    if (cameraMoved || !visiCalcFuture.valid()) {
      if (camRect != nullptr) {
        lastCamRect = *camRect;
      }
      // Launch CalcVisibleTiles asynchronously in a separate thread.
      // std::launch::async ensures it runs on a new thread immediately.
      visiCalcFuture =
          std::async(std::launch::async, &HexGrid::CalcVisibleTiles, this);
    } else {
      calcVisTime = 0.0;
    }
  }

  // Update animation frame based on game time for animated tiles.
  animationFrame = (int)(totalTime * tex::atlas::TILES_ANIMATION_SPEED) % 1;
}

void HexGrid::Update(const Camera2D &camera, float totalTime) {
  UpdateTileVisibility(totalTime);

  for (const HexCoord &h : currentVisibleTiles) {
    MapTile &tile = GetTile(h);

    // Initialise if undiscoverd and draw details
    for (TileDet &d : tile.det) {
      if (d.taOffsetX == conf::UNINITIALIZED) {
        d = GetRandomTerainDetail(tile.id);
      }
    }

    // Initialise if undiscoverd and draw resource
    rsrc::Object &rsrc = tile.rsrc;

    if (rsrc.id == rsrc::UNINITIALIZED) {
      rsrc = GetRandomTerainResource(tile.id, tile.posWorld);
    }
    if (rsrc.id != rsrc::ID_NULL) {
      if (rsrc.flashTimer > 0.0f) {
        rsrc.flashTimer -= totalTime;
        if (rsrc.flashTimer < 0.0f)
          rsrc.flashTimer = 0.0f;
      }
    }
  }
}

void HexGrid::LoadBackBuffer() {
  for (const HexCoord &h : currentVisibleTiles) {

    MapTile &tile = GetTile(h);
    tile::id id = tile.id;

    Vector2 tileCenter = HexCoordToPoint(h);
    Vector2 renderPos = Vector2{tileCenter.x - tex::size::HALF_TILE,
                                tileCenter.y - tex::size::HALF_TILE};

    Rectangle destRec = Rectangle{.x = renderPos.x,
                                  .y = renderPos.y,
                                  .width = tex::size::TILE,
                                  .height = tex::size::TILE};

    // Draw chached visible tiles
    LoadTileGFX(destRec, animationFrame + 12, id);

    // 'destRec' needs to be repostion, details and resource assets are
    // begining at the bottom
    destRec.y -= tex::size::HALF_TILE;

    // Draw details
    for (TileDet &d : tile.det) {
      if (d.taOffsetX != conf::SKIP_RENDER &&
          d.taOffsetX != conf::UNINITIALIZED) {
        LoadDetailGFX(destRec, d, tile.id);
      }
    }

    // Draw resource
    rsrc::Object &rsrc = tile.rsrc;
    if (rsrc.id != rsrc::ID_NULL && rsrc.id != rsrc::UNINITIALIZED) {
      LoadResourceGFX(destRec, rsrc, tile.id);
    }
  }
}

// ============= Render =====================
void HexGrid::LoadTileGFX(Rectangle destRec, int x, int y) {
  tex::Opts opts;
  opts.origin = {0.0f, 0.0f};
  graphicsManager->LoadTextureToBackbuffer(drawMask::GROUND0, {x, y},
                                           {destRec.x, destRec.y}, opts);
}

void HexGrid::LoadDetailGFX(Rectangle destRec, const TileDet detail,
                            tile::id id) {
  destRec.x += detail.tilePos.x;
  destRec.y += detail.tilePos.y;
  int taX = tex::atlas::DETAILS_X + detail.taOffsetX;
  tex::Opts opts;
  opts.origin = {0.0f, 0.0f};
  graphicsManager->LoadTextureToBackbuffer(drawMask::ON_GROUND, {taX, id},
                                           {destRec.x, destRec.y}, opts);
}

void HexGrid::LoadResourceGFX(Rectangle destRec, const rsrc::Object rsrc,
                              tile::id id) {
  tex::Opts opts;
  Vector2 dst = rsrc.worldPos;

  tex::atlas::Coords tex = rsrc.texAtlasCoords;

  if (rsrc.id == rsrc::ID_TREE) {
    // opts = gfx::TextureOpts32x64;
  }

  graphicsManager->LoadTextureToBackbuffer(drawMask::ON_GROUND, tex, dst, opts);
}

// ============= Getter =====================
int HexGrid::GetTilesInUse() const { return tilesInUse; }
int HexGrid::GetTilesInTotal() const { return tilesInTotal; }
int HexGrid::GetTilesVisible() const { return currentVisibleTiles.size(); }

int HexGrid::GetMapRadius() const { return mapRadius; }

HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

double HexGrid::GetVisCalcTime() const { return calcVisTime; }

bool HexGrid::CheckObstacleCollision(Vector2 worldPos, float radius) {
  HexCoord centerHex = PointToHexCoord(worldPos);

  // Check the center tile and all 6 neighbors
  std::vector<HexCoord> neighbors;
  neighbors.push_back(centerHex);
  for (int i = 0; i < 6; i++) {
    neighbors.push_back(GetNeighbor(centerHex, i));
  }

  for (const HexCoord &h : neighbors) {
    if (!HasTile(h))
      continue;

    const MapTile &tile = GetTile(h);

    const rsrc::Object &rsrc = tile.rsrc;
    if (rsrc.id != rsrc::UNINITIALIZED && rsrc.id != rsrc::ID_NULL) {
      if (rsrc.id == rsrc::ID_TREE) {

        Vector2 treePos = rsrc.worldPos;

        if (CheckCollisionCircles(worldPos, radius, treePos,
                                  conf::TREE_COLLISION_RADIUS)) {
          return true;
        }
      }
    }
  }
  return false;
}

rsrc::Object HexGrid::GetResource(HexCoord h) const {
  MapTile tile = this->GetTile(h);
  return tile.rsrc;
}
// ============= Setter =====================
void HexGrid::SetCamRectPointer(Rectangle *camRect) { this->camRect = camRect; }

bool HexGrid::SetTile(HexCoord h, tile::id id) {
  if (!IsInBounds(h) || HexCoordToType(h) == id) {
    return false;

  } else {
    MapTile &tile = GetTile(h);
    tile.id = id;
    if (id != tile::NULL_ID) {
      for (TileDet &det : tile.det) {
        det = GetRandomTerainDetail(id);
      }
    }
    rsrc::Object &rsrc = tile.rsrc;
    // r = GetRandomTerainResource(id);
    rsrc = rsrc::OBJECT_NULL;

    return true;
  }
  return false;
}

void HexGrid::Shutdown() {
  if (visiCalcFuture.valid()) {
    visiCalcFuture.wait();
  }
}
