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
}

void HexGrid::InitGrid(float radius) {

  int gridSize = mapRadius * 2 + 1;
  tiles.assign(gridSize, std::vector<MapTile>(gridSize));

  for (int r = -mapRadius; r <= mapRadius; r++) {
    for (int q = -mapRadius; q <= mapRadius; q++) {
      HexCoord h(q, r);
      int gridR = r + mapRadius;
      int gridQ = q + mapRadius;

      if (abs(q) + abs(r) + abs(-q - r) <= mapRadius * 2) {

        // 'version' should be random from 0-4, the lower the number the higher
        // the chance
        int version = 0;
        MapTile defaultTile = {.version = 0,
                               .type = TILE_GRASS,
                               .isDirty = false,
                               .isVisble = true};
        tiles[gridR][gridQ] =
            (MapTile){.type = TILE_GRASS, .isDirty = false, .isVisble = true};
        this->tilesInUse++;
      } else {
        tiles[gridR][gridQ] =
            (MapTile){.type = TILE_NULL, .isDirty = false, .isVisble = false};
      }
    }
  }
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
  return tiles[gridR][gridQ].type != TILE_NULL;
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

Vector2 HexGrid::HexCoordToPoint(HexCoord h) {
  float x = tileGapX * (sqrt(3.0f) * h.q + sqrt(3.0f) / 2.0f * h.r);
  float y = tileGapY * (3.0f / 2.0f * h.r);
  return {x + origin.x, y + origin.y};
}
TileID HexGrid::HexCoordToType(HexCoord h) {
  MapTile m = HexCoordToTile(h);
  return m.type;
}

MapTile HexGrid::HexCoordToTile(HexCoord h) {
  if (!IsInBounds(h)) {
    return (MapTile){.type = TILE_NULL, .isDirty = false, .isVisble = false};
  }
  int gridR = h.r + mapRadius;
  int gridQ = h.q + mapRadius;
  return tiles[gridR][gridQ];
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
    tiles[gridR][gridQ].type = id;
    return true;
  }
  return false;
}

void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    int gridR = h.r + mapRadius;
    int gridQ = h.q + mapRadius;
    if (tiles[gridR][gridQ].type == TILE_GRASS) {
      tiles[gridR][gridQ].type = TILE_WATER;
    } else if (tiles[gridR][gridQ].type == TILE_WATER) {
      tiles[gridR][gridQ].type = TILE_GRASS;
    }
  }
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
      if (tiles[gridR][gridQ].type ==
          TILE_NULL) { // Assuming TILE_NULL is a "wall"
        wallCount++;
      }
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::Draw(const Camera2D &camera) {
  Vector2 topLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
  Rectangle cameraView = {topLeft.x, topLeft.y, Conf::CAMERA_WIDTH,
                          Conf::CAMERA_HEIGTH};
  animationFrame = (int)(Conf::TA_TILES_ANIMATION_SPEED + GetFrameTime()) % 3;

  int gridSize = mapRadius * 2 + 1;
  for (int r = 0; r < gridSize; r++) {
    for (int q = 0; q < gridSize; q++) {
      MapTile t = tiles[r][q];
      if (t.type == TILE_NULL) {
        continue;
      }

      HexCoord h(q - mapRadius, r - mapRadius);
      Vector2 pos = HexCoordToPoint(h);
      pos.x -= Conf::TILE_SIZE_HALF;
      pos.y -= Conf::TILE_SIZE_HALF;
      Rectangle dest_rect = {pos.x, pos.y, Conf::ASSEST_RESOLUTION,
                             Conf::ASSEST_RESOLUTION};

      if (CheckCollisionRecs(cameraView, dest_rect)) {
        Rectangle tile_rect = {Conf::TA_TILE_X_OFFSET +
                                   (float)animationFrame *
                                       Conf::ASSEST_RESOLUTION,
                               (float)Conf::ASSEST_RESOLUTION * t.type,
                               Conf::ASSEST_RESOLUTION, Conf::TILE_SIZE};
        Vector2 origin = {0.0f, 0.0f};

        textureHandler->Draw(tile_rect, dest_rect, origin, 0.0f, WHITE);
      }
    }
  }
}

// --- Set/Get ---
int HexGrid::getTilesInUse() { return tilesInUse; }
int HexGrid::getMapRadius() { return mapRadius; }
HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) {
  return h + DIRECTIONS[directionIndex];
}
