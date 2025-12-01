#include "HexGrid.h"
#include "raylib.h"
#include <cmath>
#include <string>

HexGrid::HexGrid(int width, int height, float size)
    : gridWidth(width), gridHeight(height), hexSize(size) {
  for (int q = 0; q < width; q++) {
    for (int r = 0; r < height; r++) {
      hexes.push_back({q, r});
    }
  }
}

Vector2 HexGrid::HexToPixel(Hex hex) {
  Vector2 pixel;
  // pointy top
  pixel.x = hexSize * (sqrt(3.0f) * hex.q + sqrt(3.0f) / 2.0f * hex.r);
  pixel.y = hexSize * (3.0f / 2.0f * hex.r);
  // offset
  pixel.x += 100;
  pixel.y += 100;
  return pixel;
}

Hex HexGrid::PixelToHex(Vector2 pixel) {
  // offset
  pixel.x -= 100;
  pixel.y -= 100;
  // pointy top
  FractionalHex frac;
  frac.q = (sqrt(3.0f) / 3.0f * pixel.x - 1.0f / 3.0f * pixel.y) / hexSize;
  frac.r = (2.0f / 3.0f * pixel.y) / hexSize;
  return HexRound(frac);
}

Hex HexGrid::HexRound(FractionalHex frac) {
  int q = round(frac.q);
  int r = round(frac.r);
  int s = round(-frac.q - frac.r);

  float q_diff = abs(q - frac.q);
  float r_diff = abs(r - frac.r);
  float s_diff = abs(s - (-frac.q - frac.r));

  if (q_diff > r_diff && q_diff > s_diff) {
    q = -r - s;
  } else if (r_diff > s_diff) {
    r = -q - s;
  } else {
    s = -q - r;
  }

  return {q, r};
}

void HexGrid::Draw(Hex highlightedHex) {
  for (const auto &hex : hexes) {
    Vector2 center = HexToPixel(hex);

    bool isHighlighted =
        (hex.q == highlightedHex.q && hex.r == highlightedHex.r);

    DrawPoly(center, 6, hexSize, 0, isHighlighted ? YELLOW : LIGHTGRAY);
    DrawPolyLines(center, 6, hexSize, 0, BLACK);

    std::string q_str = std::to_string(hex.q);
    std::string r_str = std::to_string(hex.r);
    std::string text = q_str + "," + r_str;

    float fontSize = hexSize / 2.5f;
    Vector2 textSize =
        MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f);

    DrawText(text.c_str(), center.x - textSize.x / 2, center.y - textSize.y / 2,
             fontSize, BLACK);
  }
}
