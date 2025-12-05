#include "hex_tile_grid.h"

// --- INTERNAL COLORS ---
// Defined here locally since they are implementation details of the grid
const Color COL_HEX_EMPTY = {30, 41, 59, 255};
const Color COL_HEX_WALL = {34, 197, 94, 255};
const Color COL_HEX_GOLD = {245, 158, 11, 255};
const Color COL_HIGHLIGHT = {56, 189, 248, 255};
const Color COL_TEXT = {148, 163, 184, 255};

// =======================
//      HEX IMPLEMENTATION
// =======================

Hexagon::Hexagon() : q(0), r(0) {}
Hexagon::Hexagon(int q, int r) : q(q), r(r) {}

Hexagon Hexagon::Add(const Hexagon &other) const {
  return Hexagon(q + other.q, r + other.r);
}

bool Hexagon::Equals(const Hexagon &other) const {
  return q == other.q && r == other.r;
}

Hexagon Hexagon::operator+(const Hexagon &other) const { return Add(other); }

bool Hexagon::operator==(const Hexagon &other) const { return Equals(other); }

bool Hexagon::operator!=(const Hexagon &other) const { return !Equals(other); }

bool Hexagon::operator<(const Hexagon &other) const {
  if (q != other.q)
    return q < other.q;
  return r < other.r;
}

// ===========================
//    HEX GRID IMPLEMENTATION
// ===========================
HexGrid::HexGrid(float radius, int mapSize, Vector2 CenterPos)
    : hexRadius(radius), mapRadius(mapSize), origin(CenterPos),
      DIRECTIONS({Hexagon(1, 0), Hexagon(0, 1), Hexagon(-1, 1), Hexagon(-1, 0),
                  Hexagon(0, -1), Hexagon(1, -1)}),
      DIR_LABELS({"E", "SE", "SW", "W", "NW", "NE"}) {
  InitGrid();
}

void HexGrid::InitGrid() {
  HexTiles.clear();
  for (int q = -mapRadius; q <= mapRadius; q++) {
    int r1 = std::max(-mapRadius, -q - mapRadius);
    int r2 = std::min(mapRadius, -q + mapRadius);
    for (int r = r1; r <= r2; r++) {
      HexTiles[Hexagon(q, r)] = {Hexagon(q, r), EMPTY};
    }
  }
}

Hexagon HexGrid::HexRound(FractionalHex h) const {
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

  return Hexagon(q, r);
}

Vector2 HexGrid::HexagonToPoint(Hexagon h) const {
  float x = hexRadius * (sqrt(3.0f) * h.q + sqrt(3.0f) / 2.0f * h.r);
  float y = hexRadius * (3.0f / 2.0f * h.r);
  return {x + origin.x, y + origin.y};
}

Hexagon HexGrid::PointToHexagon(Vector2 point) const {
  float pt_x = (point.x - origin.x) / hexRadius;
  float pt_y = (point.y - origin.y) / hexRadius;
  double q = (sqrt(3.0) / 3.0 * pt_x - 1.0 / 3.0 * pt_y);
  double r = (2.0 / 3.0 * pt_y);
  return HexRound({q, r, -q - r});
}

Hexagon HexGrid::GetNeighbor(Hexagon h, int directionIndex) const {
  return h + DIRECTIONS[directionIndex];
}

bool HexGrid::HasTile(Hexagon h) const {
  return HexTiles.find(h) != HexTiles.end();
}

void HexGrid::ToggleTile(Hexagon h) {
  if (HasTile(h)) {
    HexTiles[h].type = (HexTiles[h].type == WALL) ? EMPTY : WALL;
  }
}

bool HexGrid::CheckSurrounded(Hexagon target) const {
  int neighborCount = 0;
  int wallCount = 0;

  for (int i = 0; i < 6; i++) {
    Hexagon n = GetNeighbor(target, i);
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
    Vector2 pos = HexagonToPoint(tile.coord);
    Color color = (tile.type == WALL) ? COL_HEX_WALL : COL_HEX_EMPTY;

    if (tile.type != WALL && CheckSurrounded(tile.coord)) {
      color = COL_HEX_GOLD;
    }

    DrawPoly(pos, 6, hexRadius - 2, 30, color);
    DrawText(TextFormat("%d,%d", tile.coord.q, tile.coord.r), pos.x - 12,
             pos.y - 5, 10, COL_TEXT);
  }
}

void HexGrid::DrawDebugOverlay(Vector2 mousePos) {
  Hexagon mouseHex = PointToHexagon(mousePos);

  // UI Bar background
  int screenH = GetScreenHeight();
  int screenW = GetScreenWidth();
  DrawRectangle(0, screenH - 40, screenW, 40, Fade(BLACK, 0.8f));
  DrawText(TextFormat("Mouse: %d, %d", mouseHex.q, mouseHex.r), 20,
           screenH - 30, 20, WHITE);

  if (!HasTile(mouseHex))
    return;

  Vector2 centerPx = HexagonToPoint(mouseHex);
  DrawPolyLinesEx(centerPx, 6, hexRadius, 30, 3.0f, COL_HIGHLIGHT);

  for (int i = 0; i < 6; i++) {
    Hexagon nHex = GetNeighbor(mouseHex, i);

    if (HasTile(nHex)) {
      Vector2 nPx = HexagonToPoint(nHex);

      // Connection Line
      DrawLineEx(centerPx, nPx, 2.0f, Fade(COL_HIGHLIGHT, 0.5f));

      // Label
      Vector2 midPoint = {(centerPx.x + nPx.x) / 2, (centerPx.y + nPx.y) / 2};
      DrawText(DIR_LABELS[i].c_str(), midPoint.x - 5, midPoint.y - 5, 10,
               WHITE);

      // Highlight Neighbor
      Tile nTile = HexTiles[nHex];
      Color nColor =
          (nTile.type == WALL) ? Fade(COL_HEX_WALL, 0.5f) : Fade(WHITE, 0.2f);
      DrawPoly(nPx, 6, hexRadius - 4, 30, nColor);
    }
  }
}
