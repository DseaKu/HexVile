#include "HexGrid.h"
#include "raylib.h"
#include <cmath>
#include <string>

HexGrid::HexGrid(int width, int height, float size)
    : gridWidth(width), gridHeight(height), hexSize(size) {
  // Initialize the hexagonal grid with given width and height.
  // This uses axial coordinates (q, r) where q iterates across columns
  // and r iterates across rows, effectively creating a parallelogram shape.
  for (int q = 0; q < width; q++) {
    for (int r = 0; r < height; r++) {
      hexes.push_back({q, r});
    }
  }
}

Vector2 HexGrid::HexToPixel(Hex hex) {
  Vector2 pixel;
  // Calculate pixel coordinates for a "pointy-top" hexagon orientation.
  // The coefficients (sqrt(3.0f) and 3.0f/2.0f) are derived from the
  // geometry of pointy-top hexagons in axial coordinates.
  pixel.x = hexSize * (sqrt(3.0f) * hex.q + sqrt(3.0f) / 2.0f * hex.r);
  pixel.y = hexSize * (3.0f / 2.0f * hex.r);
  // Apply an offset to center the grid on the screen or adjust its position.
  pixel.x += 100;
  pixel.y += 100;
  return pixel;
}

Hex HexGrid::PixelToHex(Vector2 pixel) {
  // Reverse the offset applied in HexToPixel to get original pixel coordinates relative to hex grid origin.
  pixel.x -= 100;
  pixel.y -= 100;
  // Convert pixel coordinates back to fractional axial hexagonal coordinates.
  // These formulas are the inverse of the HexToPixel conversion for "pointy-top" hexagons.
  FractionalHex frac;
  frac.q = (sqrt(3.0f) / 3.0f * pixel.x - 1.0f / 3.0f * pixel.y) / hexSize;
  frac.r = (2.0f / 3.0f * pixel.y) / hexSize;
  // Round the fractional hex coordinates to the nearest integer hex.
  return HexRound(frac);
}

Hex HexGrid::HexRound(FractionalHex frac) {
  // Round the fractional q, r, and s coordinates to the nearest integers.
  // The 's' coordinate is derived from q + r + s = 0.
  int q = round(frac.q);
  int r = round(frac.r);
  int s = round(-frac.q - frac.r);

  // Calculate the differences between fractional and rounded coordinates.
  float q_diff = abs(q - frac.q);
  float r_diff = abs(r - frac.r);
  float s_diff = abs(s - (-frac.q - frac.r));

  // If one difference is significantly larger than the others,
  // it indicates that the hex was closer to an edge or corner,
  // and we need to "snap" it back to a valid hex by adjusting the largest difference.
  // This is a tie-breaking mechanism for hex rounding.
  if (q_diff > r_diff && q_diff > s_diff) {
    q = -r - s;
  } else if (r_diff > s_diff) {
    r = -q - s;
  } else {
    s = -q - r; // s is adjusted, but we return {q, r}
  }

  return {q, r};
}

void HexGrid::Draw(Hex highlightedHex) {
  for (const auto &hex : hexes) {
    // Get the pixel center for each hex.
    Vector2 center = HexToPixel(hex);

    // Determine if the current hex should be highlighted.
    bool isHighlighted =
        (hex.q == highlightedHex.q && hex.r == highlightedHex.r);

    // Draw the hexagon's filled background, changing color if highlighted.
    DrawPoly(center, 6, hexSize, 0, isHighlighted ? YELLOW : LIGHTGRAY);
    // Draw the hexagon's outline.
    DrawPolyLines(center, 6, hexSize, 0, BLACK);

    // Prepare the text to display hex coordinates (q, r).
    std::string q_str = std::to_string(hex.q);
    std::string r_str = std::to_string(hex.r);
    std::string text = q_str + "," + r_str;

    // Calculate font size based on hex size for readability.
    float fontSize = hexSize / 2.5f;
    // Measure text size to center it within the hex.
    Vector2 textSize =
        MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f);

    // Draw the text (q,r coordinates) in the center of the hex.
    DrawText(text.c_str(), center.x - textSize.x / 2, center.y - textSize.y / 2,
             fontSize, BLACK);
  }
}
