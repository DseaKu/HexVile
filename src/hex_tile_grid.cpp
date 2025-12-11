#include "hex_tile_grid.h"
#include "defines.h"
#include "enums.h"
#include "raylib.h"
#include <algorithm>
#include <stdbool.h>
#include <vector>

const std::vector<HexCoord> HexGrid::DIRECTIONS = {
    HexCoord(1, 0),  HexCoord(0, 1),  HexCoord(-1, 1),
    HexCoord(-1, 0), HexCoord(0, -1), HexCoord(1, -1)};

const std::vector<TileID> HexGrid::WALKABLE_TILES = {TILE_GRASS};

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
HexGrid::HexGrid() {};

void HexGrid::InitGrid(float radius) {

  mapRadius = Conf::MAP_SIZE;
  origin = Conf::SCREEN_CENTER;
  tileGapX = Conf::TILE_GAP_X;
  tileGapY = Conf::TILE_GAP_Y;

  HexTiles.clear();
  for (int r = -this->mapRadius; r <= this->mapRadius; r++) {
    int qMin = std::max(-this->mapRadius, -r - this->mapRadius);
    int qMax = std::min(this->mapRadius, -r + this->mapRadius);
    for (int q = qMin; q <= qMax; q++) {
      HexTiles[HexCoord(q, r)] = (MapTile){
          .coord = HexCoord(q, r),
          .type = TILE_GRASS,
          .isDirty = false,
          .isVisble = false,
      };
    }
  }
}
bool HexGrid::IsWalkable(HexCoord h) const {
  MapTile tile = this->HexTiles.find(h);
  return false;
}
void HexGrid::SetTextureHandler(TextureHandler *textureHandler) {
  this->textureHandler = textureHandler;
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
  double q = (sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y);
  double r = (2.0 / 3.0 * pt_y);
  return HexRound({q, r, -q - r});
}

HexCoord HexGrid::GetNeighbor(HexCoord h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

bool HexGrid::HasTile(HexCoord h) const {
  return HexTiles.find(h) != HexTiles.end();
}

void HexGrid::SetTile(HexCoord h, TileID ID) { this->HexTiles[h].type = ID; }

void HexGrid::ToggleTile(HexCoord h) {
  if (HasTile(h)) {
    // HexTiles[h].type = (HexTiles[h].type == WALL) ? EMPTY : WALL;
    HexTiles[h].type =
        (HexTiles[h].type == TILE_GRASS) ? TILE_GRASS : TILE_WATER;
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
      if (it->second.type == TILE_NULL)
        wallCount++;
    }
  }
  return (neighborCount > 0 && wallCount == neighborCount);
}

void HexGrid::Draw(const Camera2D &camera) {
  Vector2 topLeft = GetScreenToWorld2D(Vector2{0, 0}, camera);
  Rectangle cameraView = {topLeft.x, topLeft.y, Conf::CAMERA_WIDTH,
                          Conf::CAMERA_HEIGTH};

  for (auto const &[key, tile] : HexTiles) {

    Vector2 pos = HexCoordToPoint(tile.coord);

    pos = (Vector2){pos.x - Conf::TILE_SIZE_HALF, pos.y - Conf::TILE_SIZE_HALF};

    Rectangle dest_rect = {pos.x, pos.y, Conf::ASSEST_RESOLUTION,
                           Conf::ASSEST_RESOLUTION};

    if (CheckCollisionRecs(cameraView, dest_rect)) {
      Rectangle tile_rect = {Conf::TEXTURE_ATLAS_TILES_X_OFFSET,
                             (float)Conf::ASSEST_RESOLUTION * tile.type,
                             Conf::ASSEST_RESOLUTION, Conf::TILE_SIZE};
      Vector2 origin = {0.0f, 0.0f};

      textureHandler->Draw(tile_rect, dest_rect, origin, 0.0f, WHITE);
    }
  }
}
