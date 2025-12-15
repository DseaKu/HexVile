#include "hex_tile_grid.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
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
  tileGapX = Conf::TILE_GAP_X;
  tileGapY = Conf::TILE_GAP_Y;
  origin = Conf::SCREEN_CENTER;
  mapRadius = Conf::MAP_SIZE;
  tilesInUse = 0;
  tilesInTotal = 0;
  camRect = nullptr;
  visiCacheReady = false;

  size_t estimated_hits = Conf::VISIBILTY_ESTIMATED_UPPER_BOUND;
  visiCache.reserve(estimated_hits);
  visiCacheNext.reserve(estimated_hits);
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

        // 'version' should be random from 0-4, the lower the number the higher
        // the chance
        int version = 0;
        MapTile defaultTile = {.version = 0, .type = TILE_GRASS};
        tileData[gridR][gridQ] = defaultTile;
        this->tilesInUse++;
      } else {
        tileData[gridR][gridQ] = (MapTile){.type = TILE_NULL};
      }
    }
  }
  CalcVisibleTiles();
}

void HexGrid::SetTextureHandler(TextureHandler *textureHandler) {
  this->textureHandler = textureHandler;
}

bool HexGrid::IsInBounds(HexCoord h) {
  int s = -h.q - h.r;
  return abs(h.q) <= mapRadius && abs(h.r) <= mapRadius && abs(s) <= mapRadius;
}

bool HexGrid::HasTile(HexCoord h) {
  if (!IsInBounds(h)) {
    return false;
  }
  int gridR = h.r + mapRadius;
  int gridQ = h.q + mapRadius;
  return tileData[gridR][gridQ].type != TILE_NULL;
}

bool HexGrid::IsWalkable(HexCoord h) {
  if (!HasTile(h)) {
    return false;
  }
  MapTile tile = HexCoordToTile(h);
  TileID type = tile.type;
  for (int i = 0; i < Conf::WALKABLE_TILES.size(); i++) {
    if (type == Conf::WALKABLE_TILES[i]) {
      return true;
    }
  }
  return false;
}

// --- Conversions ---
HexCoord HexGrid::HexRound(FractionalHex h) {
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

TileID HexGrid::PointToType(Vector2 point) { return PointToTile(point).type; }

Vector2 HexGrid::CoordToPoint(int r, int q) {
  float x = tileGapX * (sqrt(3.0f) * q + sqrt(3.0f) / 2.0f * r);
  float y = tileGapY * (3.0f / 2.0f * r);
  return {x + origin.x, y + origin.y};
}
Vector2 HexGrid::HexCoordToPoint(HexCoord h) { return CoordToPoint(h.r, h.q); }

TileID HexGrid::HexCoordToType(HexCoord h) {
  MapTile m = HexCoordToTile(h);
  return m.type;
}

MapTile HexGrid::HexCoordToTile(HexCoord h) {
  if (!IsInBounds(h)) {
    return (MapTile){.type = TILE_NULL};
  }
  int gridR = h.r + mapRadius;
  int gridQ = h.q + mapRadius;
  return tileData[gridR][gridQ];
}

HexCoord HexGrid::PointToHexCoord(Vector2 point) {
  float pt_x = (point.x - origin.x) / tileGapX;
  float pt_y = (point.y - origin.y) / tileGapY;
  double q = (sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y);
  double r = (2.0 / 3.0 * pt_y);
  return HexRound({q, r, -q - r});
}

MapTile HexGrid::PointToTile(Vector2 point) {
  HexCoord h = PointToHexCoord(point);
  return HexCoordToTile(h);
}
const char *HexGrid::TileToString(TileID type) {
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
bool HexGrid::SetTile(HexCoord h, TileID id) {
  if (!IsInBounds(h) || HexCoordToType(h) == id) {
    return false;
  } else {
    int gridR = h.r + mapRadius;
    int gridQ = h.q + mapRadius;
    tileData[gridR][gridQ].type = id;
    return true;
  }
  return false;
}

void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    int gridR = h.r + mapRadius;
    int gridQ = h.q + mapRadius;
    if (tileData[gridR][gridQ].type == TILE_GRASS) {
      tileData[gridR][gridQ].type = TILE_WATER;
    } else if (tileData[gridR][gridQ].type == TILE_WATER) {
      tileData[gridR][gridQ].type = TILE_GRASS;
    }
  }
}

void HexGrid::CalcVisibleTiles() {
  if (camRect == nullptr) {
    return;
  }
  // Define the rendering view rectangle, expanded by an offset for culling.
  Rectangle renderView = {.x = camRect->x - Conf::RENDER_VIEW_OFFSET_XY,
                          .y = camRect->y - Conf::RENDER_VIEW_OFFSET_XY,
                          .width = camRect->width + Conf::RENDER_VIEW_OFFSET_WH,
                          .height =
                              camRect->height + Conf::RENDER_VIEW_OFFSET_WH};

  int gridSize = mapRadius * 2 + 1;

  std::vector<VisibiltyData> newVisiCache;
  newVisiCache.reserve(
      Conf::VISIBILTY_ESTIMATED_UPPER_BOUND); // Pre-allocate memory for
                                              // efficiency.

  // Iterate over all tiles in the grid.
  for (u32 r = 0; r < gridSize; r++) {
    for (u32 q = 0; q < gridSize; q++) {

      if (tileData[r][q].type == TILE_NULL) {
        continue;
      }
      // Convert grid coordinates to HexCoord and then to screen coordinates.
      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= Conf::TILE_SIZE_HALF;
      pos.y -= Conf::TILE_SIZE_HALF;
      Rectangle dest_rect = {pos.x, pos.y, Conf::ASSEST_RESOLUTION,
                             Conf::ASSEST_RESOLUTION};
      // Check if the tile's bounding box intersects with the render view.
      if (CheckCollisionRecs(renderView, dest_rect)) {
        newVisiCache.push_back((VisibiltyData){.r = r, .q = q});
      }
    }
  }
  // Lock the mutex to safely swap the newly calculated visible tiles into the
  // visiCacheNext. std::lock_guard ensures the mutex is unlocked when exiting
  // this scope.
  std::lock_guard<std::mutex> lock(visiCacheMutex);
  visiCacheNext =
      std::move(newVisiCache); // Move the new data to the back buffer.
  visiCacheReady = true; // Signal that new data is ready for the main thread.
}

bool HexGrid::CheckSurrounded(HexCoord target) {
  if (!IsInBounds(target)) {
    return false;
  }

  int neighborCount = 0;
  int wallCount = 0;

  for (int i = 0; i < 6; i++) {
    HexCoord n = GetNeighbor(target, i);
    if (IsInBounds(n)) {
      neighborCount++;
      int gridR = n.r + mapRadius;
      int gridQ = n.q + mapRadius;
      if (tileData[gridR][gridQ].type == TILE_NULL) {
        wallCount++;
      }
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::Draw(const Camera2D &camera) {
  // Check if the asynchronous calculation of visible tiles is complete and new
  // data is ready.
  if (visiCacheReady) {
    // Lock the mutex to safely swap the current rendering cache with the newly
    // calculated one.
    std::lock_guard<std::mutex> lock(visiCacheMutex);
    visiCache.swap(visiCacheNext); // Atomically swap the buffers.
    visiCacheNext.clear();         // Clear the now-empty back buffer.
    visiCacheReady = false;        // Reset the flag.
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
  animationFrame = (int)(GetTime() * Conf::TA_TILES_ANIMATION_SPEED) %
                   Conf::TA_TILES_FRAME_COUNT;

  // Draw chached visible tiles
  for (int i = 0; i < visiCache.size(); i++) {
    int q = visiCache[i].q;
    int r = visiCache[i].r;
    HexCoord h(q - mapRadius, r - mapRadius);
    Vector2 pos = HexCoordToPoint(h);
    pos.x -= Conf::TILE_SIZE_HALF;
    pos.y -= Conf::TILE_SIZE_HALF;
    Rectangle dest_rect = {pos.x, pos.y, Conf::ASSEST_RESOLUTION,
                           Conf::ASSEST_RESOLUTION};

    Rectangle tile_rect = {Conf::TA_TILE_X_OFFSET +
                               (float)animationFrame * Conf::ASSEST_RESOLUTION,
                           (float)Conf::ASSEST_RESOLUTION * tileData[r][q].type,
                           Conf::ASSEST_RESOLUTION, Conf::TILE_SIZE};
    Vector2 origin = {0.0f, 0.0f};

    textureHandler->Draw(tile_rect, dest_rect, origin, 0.0f, WHITE);
  }
}

// --- Get ---
int HexGrid::GetTilesInUse() { return tilesInUse; }
int HexGrid::GetTilesInTotal() { return tilesInTotal; }
int HexGrid::GetTilesVisible() { return visiCache.size(); }

int HexGrid::GetMapRadius() { return mapRadius; }
HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) {
  return h + DIRECTIONS[directionIndex];
}

// --- Set ---
void HexGrid::SetCamRectPointer(Rectangle *camRect) { this->camRect = camRect; }
