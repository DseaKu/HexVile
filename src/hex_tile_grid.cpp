#include "hex_tile_grid.h"
#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include "texture_atlas.h"
#include <algorithm>
#include <vector>

const std::vector<HexCoord> HexGrid::DIRECTIONS = {
    HexCoord(1, 0),  HexCoord(0, 1),  HexCoord(-1, 1),
    HexCoord(-1, 0), HexCoord(0, -1), HexCoord(1, -1)};

// --- Hex ---
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

// --- Hex Grid ---
HexGrid::HexGrid() {

  animationFrame = 0;
  tileGapX = Conf::TILE_SPACING_X;
  tileGapY = Conf::TILE_SPACING_Y;
  origin = Conf::SCREEN_CENTER;
  mapRadius = Conf::MAP_RADIUS;
  tilesInUse = 0;
  tilesInTotal = 0;
  camRect = nullptr;
  visiCacheReady = false;

  // Gaussian distribution for terrain detail generation
  std::random_device rd;
  randomEngine = std::mt19937(rd());

  const float mean = (TA::DETAILS_X + TA::DETAILS_X_MAX - 1) / 2.0f;
  const float stddev = (TA::DETAILS_X_MAX - TA::DETAILS_X) / 6.0f;
  typeDistribution = std::normal_distribution<float>(mean, stddev);

  size_t estimated_hits = Conf::ESTIMATED_VISIBLE_TILES;
  currentVisibleTiles.reserve(estimated_hits);
  nextVisibleTiles.reserve(estimated_hits);
}

void HexGrid::InitGrid(float radius) {

  int gridSize = mapRadius * 2 + 1;
  tileData.assign(gridSize, std::vector<MapTile>(gridSize));

  // Init tile data
  for (int r = -mapRadius; r <= mapRadius; r++) {
    for (int q = -mapRadius; q <= mapRadius; q++) {
      HexCoord h(q, r);
      int gridR = r + mapRadius;
      int gridQ = q + mapRadius;
      this->tilesInTotal++;

      if (abs(q) + abs(r) + abs(-q - r) <= mapRadius * 2) {

        MapTile initTile = {.type = TILE_GRASS};

        for (TerrainDetail &d : initTile.detail) {
          d = GetRandomTerainDetail();
        }

        tileData[gridR][gridQ] = initTile;
        this->tilesInUse++;
      } else {
        tileData[gridR][gridQ] = (MapTile){.type = TILE_NULL};
      }
    }
  }
  CalcVisibleTiles();
}

TerrainDetail HexGrid::GetRandomTerainDetail() {
  float x = GetRandomValue(-TA::RES16 / 2, TA::RES16 / 2);
  float y = GetRandomValue(-TA::RES16 / 2, TA::RES16 / 2);

  // Generate a value from the normal distribution
  float generated_type_float = typeDistribution(randomEngine);

  // Round to nearest integer
  int generated_type = static_cast<int>(std::round(generated_type_float));

  // Clamp the value to the valid range
  int type = std::clamp(generated_type, TA::DETAILS_X, TA::DETAILS_X_MAX - 1);

  return TerrainDetail{.x = x, .y = y, .type = type};
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
  return GetTile(h).type != TILE_NULL;
}

bool HexGrid::IsWalkable(HexCoord h) const {
  if (!HasTile(h)) {
    return false;
  }
  MapTile tile = HexCoordToTile(h);
  TileID type = tile.type;
  for (int i = 0; i < Conf::WALKABLE_TILE_IDS.size(); i++) {
    if (type == Conf::WALKABLE_TILE_IDS[i]) {
      return true;
    }
  }
  return false;
}

const MapTile &HexGrid::GetTile(HexCoord h) const {
  return tileData[h.r + mapRadius][h.q + mapRadius];
}

MapTile &HexGrid::GetTile(HexCoord h) {
  return tileData[h.r + mapRadius][h.q + mapRadius];
}

// --- Conversions ---
HexCoord HexGrid::HexRound(FractionalHex h) const {
  int q = round(h.q);
  int r = round(h.r);
  int s = round(h.s);
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
  float x = tileGapX * (sqrt(3.0f) * q + sqrt(3.0f) / 2.0f * r);
  float y = tileGapY * (3.0f / 2.0f * r);
  return {x + origin.x, y + origin.y};
}

TileID HexGrid::PointToType(Vector2 point) const {
  return PointToTile(point).type;
}

TileID HexGrid::HexCoordToType(HexCoord h) const {
  MapTile m = HexCoordToTile(h);
  return m.type;
}

MapTile HexGrid::HexCoordToTile(HexCoord h) const {
  if (!IsInBounds(h)) {
    return (MapTile){.type = TILE_NULL};
  }
  return GetTile(h);
}

MapTile HexGrid::PointToTile(Vector2 point) const {
  HexCoord h = PointToHexCoord(point);
  return HexCoordToTile(h);
}

HexCoord HexGrid::PointToHexCoord(Vector2 point) const {
  float pt_x = (point.x - origin.x) / tileGapX;
  float pt_y = (point.y - origin.y) / tileGapY;
  double q = (sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y);
  double r = (2.0 / 3.0 * pt_y);
  return HexRound({q, r, -q - r});
}

const char *HexGrid::TileToString(TileID type) const {
  switch (type) {
  case TILE_NULL:
    return "NULL";
  case TILE_GRASS:
    return "Grass";
  case TILE_WATER:
    return "Water";
  case TILE_DIRT:
    return "Dirt";
  default:
    return "Undefined";
  }
}

// --- Logic ---
void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    MapTile &tile = GetTile(h);
    if (tile.type == TILE_GRASS) {
      tile.type = TILE_WATER;
    } else if (tile.type == TILE_WATER) {
      tile.type = TILE_GRASS;
    }
  }
}

void HexGrid::CalcVisibleTiles() {
  if (camRect == nullptr) {
    return;
  }
  // Define the rendering view rectangle, expanded by an offset for culling.
  Rectangle renderView = {
      .x = camRect->x - Conf::RENDER_VIEW_CULLING_MARGIN,
      .y = camRect->y - Conf::RENDER_VIEW_CULLING_MARGIN,
      .width = camRect->width + Conf::RENDER_VIEW_CULLING_EXPANSION,
      .height = camRect->height + Conf::RENDER_VIEW_CULLING_EXPANSION};

  int gridSize = mapRadius * 2 + 1;

  std::vector<HexCoord> newVisiCache;
  newVisiCache.reserve(Conf::ESTIMATED_VISIBLE_TILES); // Pre-allocate memory
                                                       // for efficiency.

  // Iterate over all tiles in the grid.
  for (u16 r = 0; r < gridSize; r++) {
    for (u16 q = 0; q < gridSize; q++) {

      if (tileData[r][q].type == TILE_NULL) {
        continue;
      }
      // Convert grid coordinates to HexCoord and then to screen coordinates.
      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= Conf::TILE_RESOLUTION_HALF;
      pos.y -= Conf::TILE_RESOLUTION_HALF;
      Rectangle dest_rect = {pos.x, pos.y, TA::RES, TA::RES};
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
      if (GetTile(n).type == TILE_NULL) {
        wallCount++;
      }
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::DrawTile(HexCoord h, int TA_X, int TA_Y, DrawMaskID layer) {
  if (!HasTile(h)) {
    return;
  }
  Vector2 pos = HexCoordToPoint(h);
  pos.x -= Conf::TILE_RESOLUTION_HALF;
  pos.y -= Conf::TILE_RESOLUTION_HALF;
  Rectangle destRect = {pos.x, pos.y, TA::RES, TA::RES};

  const MapTile &tile = GetTile(h);
  Vector2 origin = {0.0f, 0.0f};

  graphicsManager->LoadGFX_Data(layer, destRect.y, TA_X, TA_Y, destRect, WHITE);
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
    // Launch CalcVisibleTiles asynchronously in a separate thread.
    // std::launch::async ensures it runs on a new thread immediately.
    visiCalcFuture =
        std::async(std::launch::async, &HexGrid::CalcVisibleTiles, this);
  }

  // Update animation frame based on game time for animated tiles.
  animationFrame = (int)(totalTime * TA::TILES_ANIMATION_SPEED) % 1;
}

void HexGrid::Update(const Camera2D &camera, float totalTime) {
  UpdateTileVisibility(totalTime);
  LoadTileGFX();
  // AddGrassDetails(200);
}

// --- Render ---
void HexGrid::LoadTileGFX() {
  // Draw chached visible tiles
  for (int i = 0; i < currentVisibleTiles.size(); i++) {
    HexCoord h = currentVisibleTiles[i];
    Vector2 pos = HexCoordToPoint(h);
    pos.x -= Conf::TILE_RESOLUTION_HALF;
    pos.y -= Conf::TILE_RESOLUTION_HALF;
    Rectangle destRec = {pos.x, pos.y, TA::RES, TA::RES};

    MapTile &t = GetTile(h);
    int x = animationFrame + 12;
    int y = t.type;

    graphicsManager->LoadGFX_Data(DRAW_MASK_GROUND_0, destRec.y, x, y, destRec,
                                  WHITE);

    // Draw details for this tile
    for (const TerrainDetail &d : t.detail) {
      if (d.type != 0) {
        LoadDetailGFX(d, pos.x, pos.y, t.type);
      }
    }
  }
}

void HexGrid::LoadDetailGFX(const TerrainDetail d, float x, float y,
                            TileID type) {

  Rectangle destRec = {x + d.x, y + d.y - TA::RES16_F, TA::RES, TA::RES};

  graphicsManager->LoadGFX_Data(DRAW_MASK_ON_GROUND, y - TA::RES16_F, d.type,
                                type, destRec, WHITE);
}

// --- Get ---
int HexGrid::GetTilesInUse() const { return tilesInUse; }
int HexGrid::GetTilesInTotal() const { return tilesInTotal; }
int HexGrid::GetTilesVisible() const { return currentVisibleTiles.size(); }

int HexGrid::GetMapRadius() const { return mapRadius; }

HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

// --- Set ---
void HexGrid::SetCamRectPointer(Rectangle *camRect) { this->camRect = camRect; }

bool HexGrid::SetTile(HexCoord h, TileID id) {
  if (!IsInBounds(h) || HexCoordToType(h) == id) {
    return false;

  } else {
    MapTile &t = GetTile(h);
    t.type = id;
    for (TerrainDetail &d : t.detail) {
      d = GetRandomTerainDetail();
    }
    return true;
  }
  return false;
}

void HexGrid::Shutdown() {
  if (visiCalcFuture.valid()) {
    visiCalcFuture.wait();
  }
}
