#ifndef HEXGRID_H
#define HEXGRID_H

#include "raylib.h"
#include <vector>

// Hexagonal grid using axial coordinates
struct Hex {
    int q;
    int r;

    bool operator==(const Hex& other) const {
        return q == other.q && r == other.r;
    }
};

struct FractionalHex {
    float q;
    float r;
};

class HexGrid {
public:
    HexGrid(int width, int height, float hexSize);
    void Draw(Hex highlightedHex);
    Vector2 HexToPixel(Hex hex);
    Hex PixelToHex(Vector2 pixel);
    Hex HexRound(FractionalHex frac);
    
    std::vector<Hex> hexes;

private:
    int gridWidth;
    int gridHeight;
    float hexSize;
};

#endif // HEXGRID_H
