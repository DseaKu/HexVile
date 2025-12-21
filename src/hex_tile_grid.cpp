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
  tileGapX = conf::TILE_SPACING_X;
  tileGapY = conf::TILE_SPACING_Y;
  origin = conf::SCREEN_CENTER;
  mapRadius = conf::MAP_RADIUS;
  gridSize = 0;
  tilesInUse = 0;
  tilesInTotal = 0;
  camRect = nullptr;
  visiCacheReady = false;

  // Gaussian distribution for terrain detail generation
  std::random_device rd;
  randomEngine = std::mt19937(rd());

  const float mean = (ta::DETAILS_X + ta::DETAILS_X_MAX - 1) / 2.0f;
  const float stddev =
      (ta::DETAILS_X_MAX - ta::DETAILS_X) / conf::GAUSIAN_EFFECT;
  detailDistribution = std::normal_distribution<float>(mean, stddev);

  const float meanRes = (ta::RESOURCE_X + ta::RESOURCE_X_MAX - 1) / 2.0f;
  const float stddevRes =
      (ta::RESOURCE_X_MAX - ta::RESOURCE_X) / conf::GAUSIAN_EFFECT;
  resourceDistribution = std::normal_distribution<float>(meanRes, stddevRes);

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

      if (abs(q) + abs(r) + abs(-q - r) <= mapRadius * 2) {

        MapTile initTile = {.tileID = tile::GRASS};

        for (TerDet &d : initTile.det) {
          d.detID = conf::UNINITIALIZED;
        }

        for (TerRes &r : initTile.res) {
          r.resID = conf::UNINITIALIZED;
        }

        tileData[gridR * gridSize + gridQ] = initTile;
        this->tilesInUse++;

      } else {
        tileData[gridR * gridSize + gridQ] = (MapTile){.tileID = tile::NULL_ID};
      }
    }
  }
  CalcVisibleTiles();
}

TerDet HexGrid::GetRandomTerainDetail(tile::id tileID) {
  float x = GetRandomValue(-ta::RES8_F, ta::RES8_F);
  float y = GetRandomValue(-ta::RES8_F, ta::RES8_F);

  // Generate a value from the normal distribution
  float generated_type_float = detailDistribution(randomEngine);

  // Round to nearest integer
  int generated_type = static_cast<int>(std::round(generated_type_float));

  // Clamp the value to the valid range
  int type = std::clamp(generated_type, ta::DETAILS_X, ta::DETAILS_X_MAX - 1);

  // Determine if detail is null and skip render process
  int renderBitMask = ta::RENDER_BIT_MASK_DETAIL.at(tileID);
  int detailShift = type - ta::DETAILS_X;
  if (!(renderBitMask >> detailShift & 1)) {
    type = conf::SKIP_RENDER;
  }

  return TerDet{.x = x, .y = y, .detID = type};
}

TerRes HexGrid::GetRandomTerainResource(tile::id tileID) {
  float x = GetRandomValue(-ta::RES8_F, ta::RES8_F);
  float y = GetRandomValue(-ta::RES8_F, ta::RES8_F);

  // Generate a value from the normal distribution
  float generated_type_float = resourceDistribution(randomEngine);

  // Round to nearest integer
  int generated_type = static_cast<int>(std::round(generated_type_float));

  // Clamp the value to the valid range
  int type = std::clamp(generated_type, ta::RESOURCE_X, ta::RESOURCE_X_MAX - 1);

  // Determine if resource is null and skip render process
  int renderBitMask = ta::RENDER_BIT_MASK_RESOURCE.at(tileID);
  int resourceShift = type - ta::RESOURCE_X;
  if (!(renderBitMask >> resourceShift & 1)) {
    type = conf::SKIP_RENDER;
  }

  return TerRes{.x = x, .y = y, .resID = type};
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
  return GetTile(h).tileID != tile::NULL_ID;
}

bool HexGrid::IsWalkable(HexCoord h) const {
  if (!HasTile(h)) {
    return false;
  }
  MapTile tile = HexCoordToTile(h);
  tile::id type = tile.tileID;
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

tile::id HexGrid::PointToType(Vector2 point) const {
  return PointToTile(point).tileID;
}

tile::id HexGrid::HexCoordToType(HexCoord h) const {
  MapTile m = HexCoordToTile(h);
  return m.tileID;
}

MapTile HexGrid::HexCoordToTile(HexCoord h) const {
  if (!IsInBounds(h)) {
    return (MapTile){.tileID = tile::NULL_ID};
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

const char *HexGrid::TileToString(tile::id tileID) const {
  switch (tileID) {
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

// --- Logic ---
void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    MapTile &tile = GetTile(h);
    if (tile.tileID == tile::GRASS) {
      tile.tileID = tile::WATER;
    } else if (tile.tileID == tile::WATER) {
      tile.tileID = tile::GRASS;
    }
  }
}

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

      if (tileData[r * gridSize + q].tileID == tile::NULL_ID) {
        continue;
      }
      // Convert grid coordinates to HexCoord and then to screen coordinates.
      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= conf::TILE_RESOLUTION_HALF;
      pos.y -= conf::TILE_RESOLUTION_HALF;
      Rectangle dest_rect = {pos.x, pos.y, ta::RES32, ta::RES32};
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
      if (GetTile(n).tileID == tile::NULL_ID) {
        wallCount++;
      }
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::DrawTile(HexCoord h, int TA_X, int TA_Y, drawMask::id layerID) {
  if (!HasTile(h)) {
    return;
  }
  Vector2 pos = HexCoordToPoint(h);
  pos.x -= conf::TILE_RESOLUTION_HALF;
  pos.y -= conf::TILE_RESOLUTION_HALF;
  Rectangle destRect = {pos.x, pos.y, ta::RES32, ta::RES32};

  const MapTile &tile = GetTile(h);
  Vector2 origin = {0.0f, 0.0f};

  graphicsManager->LoadGFX_Data(layerID, destRect.y, TA_X, TA_Y, destRect,
                                WHITE);
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
  animationFrame = (int)(totalTime * ta::TILES_ANIMATION_SPEED) % 1;
}

void HexGrid::Update(const Camera2D &camera, float totalTime) {
  UpdateTileVisibility(totalTime);

  for (const HexCoord &h : currentVisibleTiles) {

    MapTile &t = GetTile(h);
    tile::id id = t.tileID;

    Vector2 tileCenter = HexCoordToPoint(h);
    Vector2 renderPos =
        Vector2{tileCenter.x - ta::RES16_F, tileCenter.y - ta::RES16_F};

    Rectangle destRec = Rectangle{.x = renderPos.x,
                                  .y = renderPos.y,
                                  .width = ta::RES32_F,
                                  .height = ta::RES32_F};

    // Draw chached visible tiles
    LoadTileGFX(destRec, animationFrame + 12, id);

    // 'destRec' needs to be repostion, details and resource assets are
    // begining at the bottom
    destRec.y -= ta::RES16_F;

    // Initialise if undiscoverd and draw details
    for (TerDet &d : t.det) {
      if (d.detID == conf::UNINITIALIZED) {
        d = GetRandomTerainDetail(t.tileID);
      }
      if (d.detID != conf::SKIP_RENDER) {
        LoadDetailGFX(destRec, d, t.tileID);
      }
    }

    // Initialise if undiscoverd and draw resource
    for (TerRes &r : t.res) {
      if (r.resID == conf::UNINITIALIZED) {
        r = GetRandomTerainResource(t.tileID);
      }
      if (r.resID != conf::SKIP_RENDER) {
        LoadResourceGFX(destRec, r, t.tileID);
      }
    }
  }
}

// --- Render ---
void HexGrid::LoadTileGFX(Rectangle destRec, int x, int y) {
  graphicsManager->LoadGFX_Data(drawMask::GROUND_0, destRec.y, x, y, destRec,
                                WHITE);
}

void HexGrid::LoadDetailGFX(Rectangle destRec, const TerDet d,
                            tile::id tileID) {
  graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, destRec.y, d.detID, tileID,
                                destRec, WHITE);
}

void HexGrid::LoadResourceGFX(Rectangle destRec, const TerRes r,
                              tile::id tileID) {
  if (r.resID - ta::RESOURCE_X == res::TREE) {
    destRec.height += ta::RES32_F;
    graphicsManager->LoadGFX_Data_32x64(drawMask::ON_GROUND, destRec.y, r.resID,
                                        tileID, destRec, WHITE);
  } else {
    graphicsManager->LoadGFX_Data(drawMask::ON_GROUND, destRec.y, r.resID,
                                  tileID, destRec, WHITE);
  }
}
// --- Get ---
int HexGrid::GetTilesInUse() const { return tilesInUse; }
int HexGrid::GetTilesInTotal() const { return tilesInTotal; }
int HexGrid::GetTilesVisible() const { return currentVisibleTiles.size(); }

int HexGrid::GetMapRadius() const { return mapRadius; }

HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

double HexGrid::GetVisCalcTime() const { return calcVisTime; }

// --- Set ---
void HexGrid::SetCamRectPointer(Rectangle *camRect) { this->camRect = camRect; }

bool HexGrid::SetTile(HexCoord h, tile::id tileID) {
  if (!IsInBounds(h) || HexCoordToType(h) == tileID) {
    return false;

  } else {
    MapTile &t = GetTile(h);
    t.tileID = tileID;
    for (TerDet &d : t.det) {
      d = GetRandomTerainDetail(tileID);
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
