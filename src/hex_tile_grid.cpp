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
  for (int i = 0; i < Conf::WALKABLE_TILES.size(); i++) {
    if (type == Conf::WALKABLE_TILES[i]) {
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
  float startTime = GetTime();
  // Define the rendering view rectangle, expanded by an offset for culling.
  Rectangle renderView = {.x = camRect->x - Conf::RENDER_VIEW_OFFSET_XY,
                          .y = camRect->y - Conf::RENDER_VIEW_OFFSET_XY,
                          .width = camRect->width + Conf::RENDER_VIEW_OFFSET_WH,
                          .height =
                              camRect->height + Conf::RENDER_VIEW_OFFSET_WH};

  int gridSize = mapRadius * 2 + 1;

  std::vector<HexCoord> newVisiCache;
  newVisiCache.reserve(
      Conf::VISIBILTY_ESTIMATED_UPPER_BOUND); // Pre-allocate memory for
                                              // efficiency.

  // Iterate over all tiles in the grid.
  for (u16 r = 0; r < gridSize; r++) {
    for (u16 q = 0; q < gridSize; q++) {

      if (tileData[r][q].type == TILE_NULL) {
        continue;
      }
      // Convert grid coordinates to HexCoord and then to screen coordinates.
      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= Conf::TILE_SIZE_HALF;
      pos.y -= Conf::TILE_SIZE_HALF;
      Rectangle dest_rect = {pos.x, pos.y, TA::ASSEST_RESOLUTION,
                             TA::ASSEST_RESOLUTION};
      // Check if the tile's bounding box intersects with the render view.
      if (CheckCollisionRecs(renderView, dest_rect)) {
        newVisiCache.push_back(h);
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
  calcRenderRectTimer = GetTime() - startTime;
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

void HexGrid::DrawTile(Vector2 point) { DrawTile(PointToHexCoord(point)); }

void HexGrid::DrawTile(HexCoord h) {
  if (!HasTile(h)) {
    return;
  }
  Vector2 pos = HexCoordToPoint(h);
  pos.x -= Conf::TILE_SIZE_HALF;
  pos.y -= Conf::TILE_SIZE_HALF;
  Rectangle destRect = {pos.x, pos.y, TA::ASSEST_RESOLUTION,
                        TA::ASSEST_RESOLUTION};

  const MapTile &tile = GetTile(h);
  Rectangle sourceRect = {TA::TILE_X_OFFSET_TILE +
                              (float)animationFrame * TA::ASSEST_RESOLUTION,
                          (float)TA::ASSEST_RESOLUTION * tile.type,
                          TA::ASSEST_RESOLUTION, Conf::TILE_SIZE};
  Vector2 origin = {0.0f, 0.0f};

  textureHandler->Draw(sourceRect, destRect, origin, 0.0f, RED);
}

void HexGrid::DrawVisibleTiles() {
  // Draw chached visible tiles
  for (int i = 0; i < visiCache.size(); i++) {
    HexCoord h = visiCache[i];
    Vector2 pos = HexCoordToPoint(h);
    pos.x -= Conf::TILE_SIZE_HALF;
    pos.y -= Conf::TILE_SIZE_HALF;
    Rectangle destRect = {pos.x, pos.y, TA::ASSEST_RESOLUTION,
                          TA::ASSEST_RESOLUTION};

    const MapTile &tile = GetTile(h);
    Rectangle sourceRect = {TA::TILE_X_OFFSET_TILE +
                                (float)animationFrame * TA::ASSEST_RESOLUTION,
                            (float)TA::ASSEST_RESOLUTION * tile.type,
                            TA::ASSEST_RESOLUTION, Conf::TILE_SIZE};
    Vector2 origin = {0.0f, 0.0f};

    textureHandler->Draw(sourceRect, destRect, origin, 0.0f, WHITE);

    // Draw details for this tile
    const MapTile &currentTile = tile;
    for (int j = 0; j < Conf::TERRAIN_DETAIL_MAX; j++) {
      const TerrainDetail &d = currentTile.detail[j];
      if (d.detail > 0) { // Assuming detail ID > 0 is a valid detail

        Rectangle detailSourceRect = {
            TA::DETAILS_X_OFFSET_TILE +
                ((float)d.detail - 1) * TA::ASSEST_RESOLUTION,
            (float)ITEM_SET_GRASS * TA::ASSEST_RESOLUTION,
            TA::ASSEST_RESOLUTION, TA::ASSEST_RESOLUTION};

        Rectangle detailDestRect = {pos.x + d.x, pos.y + d.y,
                                    TA::ASSEST_RESOLUTION,
                                    TA::ASSEST_RESOLUTION};

        textureHandler->Draw(detailSourceRect, detailDestRect, origin, 0.0f,
                             WHITE);
      }
    }
  }
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
  animationFrame =
      (int)(GetTime() * TA::TILES_ANIMATION_SPEED) % TA::TILES_FRAME_COUNT;

  DrawVisibleTiles();
}

void HexGrid::AddGrassDetails(int amount) {
  int detailsAdded = 0;
  int attempts = 0;
  // Try to add 'amount' details, with a limit on attempts to avoid
  // infinite loops
  while (detailsAdded < amount && attempts < amount * 10) {
    attempts++;
    int q_rand = GetRandomValue(0, mapRadius * 2);
    int r_rand = GetRandomValue(0, mapRadius * 2);

    // Only add details to grass tiles.
    if (tileData[r_rand][q_rand].type != TILE_GRASS) {
      continue;
    }

    // Generate smaller, positive-only offsets to keep details within tile
    // bounds. The previous range of [-16, 16] was causing issues with the `u8`
    // type and placing details far outside the tile. This range is a guess;
    // you may need to adjust it based on your asset sizes.
    int x_pos_rand = GetRandomValue(1, 15);
    int y_pos_rand = GetRandomValue(1, 15);
    int index_rand = GetRandomValue(0, Conf::TERRAIN_DETAIL_MAX - 1);
    int detailRand = GetRandomValue(GRASS_DETAILS_FLOWER, GRASS_DETAILS_SIZE);

    // Only add detail if the slot is empty.
    if (tileData[r_rand][q_rand].detail[index_rand].detail == 0) {
      tileData[r_rand][q_rand].detail[index_rand] = {
          .x = (u8)x_pos_rand, .y = (u8)y_pos_rand, .detail = detailRand};
      detailsAdded++;
    }
  }
}

void HexGrid::UpdateGrid() { AddGrassDetails(200); }

// --- Get ---
int HexGrid::GetTilesInUse() const { return tilesInUse; }
int HexGrid::GetTilesInTotal() const { return tilesInTotal; }
int HexGrid::GetTilesVisible() const { return visiCache.size(); }

int HexGrid::GetMapRadius() const { return mapRadius; }

float HexGrid::GetRenderRectTimer() const { return calcRenderRectTimer; }
HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

// --- Set ---
void HexGrid::SetCamRectPointer(Rectangle *camRect) { this->camRect = camRect; }

bool HexGrid::SetTile(HexCoord h, TileID id) {
  if (!IsInBounds(h) || HexCoordToType(h) == id) {
    return false;
  } else {
    MapTile &tile = GetTile(h);
    tile.type = id;
    for (int i = 0; i < Conf::TERRAIN_DETAIL_MAX; i++) {
      tile.detail[i] = {0};
    }
    return true;
  }
  return false;
}
