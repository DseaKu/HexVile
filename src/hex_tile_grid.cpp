#include "hex_tile_grid.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <vector>

// --- INTERNAL COLORS ---
// Defined here locally since they are implementation details of the grid
const Color COL_HEX_EMPTY = {30, 41, 59, 255};
const Color COL_HEX_WALL = {34, 197, 94, 255};
const Color COL_HEX_GOLD = {245, 158, 11, 255};
const Color COL_HIGHLIGHT = {56, 189, 248, 255};
const Color COL_TEXT = {148, 163, 184, 255};

const std::vector<HexCoord> HexGrid::DIRECTIONS = {
    HexCoord(1, 0),  HexCoord(0, 1),  HexCoord(-1, 1),
    HexCoord(-1, 0), HexCoord(0, -1), HexCoord(1, -1)};

const std::vector<std::string> HexGrid::DIR_LABELS = {"E", "SE", "SW",
                                                      "W", "NW", "NE"};

// =======================
//      HEX IMPLEMENTATION
// =======================

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

// ===========================
//    HEX GRID IMPLEMENTATION
// ===========================
HexGrid::HexGrid() {};

void HexGrid::InitGrid(float radius) {

  mapRadius = Config::MAP_SIZE;
  origin = Config::SCREEN_CENTER;
  tileGapX = Config::TILE_GAP_X;
  tileGapY = Config::TILE_GAP_Y;

  HexTiles.clear();
  for (int r = -this->mapRadius; r <= this->mapRadius; r++) {
    int qMin = std::max(-this->mapRadius, -r - this->mapRadius);
    int qMax = std::min(this->mapRadius, -r + this->mapRadius);
    for (int q = qMin; q <= qMax; q++) {
      HexTiles[HexCoord(q, r)] = {.coord = HexCoord(q, r), .type = EMPTY};
    }
  }
}

void HexGrid::LoadAssets(const char *pathToAssets) {

  tileAssets = LoadTexture(pathToAssets);
}

HexCoord HexGrid::HexRound(FractionalHex h) const {
  int q = (int)round(h.q);
  int r = (int)round(h.r);
  int s = (int)round(h.s);
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
  float x = tileGapX * (sqrt(3.0f) * h.q + sqrt(3.0f) / 2.0f * h.r);
  float y = tileGapY * (3.0f / 2.0f * h.r);
  return {x + origin.x, y + origin.y};
}

HexCoord HexGrid::PointToHexCoord(Vector2 point) const {
  float pt_x = (point.x - origin.x) / tileGapX;
  float pt_y = (point.y - origin.y) / tileGapY;
  double q = (sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y) / Config::CAMERA_ZOOM;
  double r = (2.0 / 3.0 * pt_y) / Config::CAMERA_ZOOM;
  return HexRound({q, r, -q - r});
}

HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

bool HexGrid::HasTile(HexCoord h) const {
  return HexTiles.find(h) != HexTiles.end();
}

void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    HexTiles[h].type = (HexTiles[h].type == WALL) ? EMPTY : WALL;
  }
}

bool HexGrid::CheckSurrounded(HexCoord target) const {
  int neighborCount = 0;
  int wallCount = 0;

  for (int i = 0; i < 6; i++) {
    HexCoord n = GetNeighbor(target, i);
    auto it = HexTiles.find(n);

    if (it != HexTiles.end()) {
      neighborCount++;
      if (it->second.type == WALL)
        wallCount++;
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::Draw() {
  for (auto const &[key, tile] : HexTiles) {
    Vector2 pos = HexCoordToPoint(tile.coord);

    pos = (Vector2){pos.x - Config::TILE_SIZE_HALF,
                    pos.y - Config::TILE_SIZE_HALF};

    Rectangle tile_rect = {(float)Config::TILE_SIZE * 0,
                           (float)Config::TILE_SIZE * tile.type,
                           Config::TILE_SIZE, Config::TILE_SIZE};
    Rectangle dest_rect = {pos.x, pos.y, Config::TILE_SIZE, Config::TILE_SIZE};
    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(tileAssets, tile_rect, dest_rect, origin, 0.0f, WHITE);
  }
}

void HexGrid::UnloadAssets() { UnloadTexture(tileAssets); }
