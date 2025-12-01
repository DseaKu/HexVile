#ifndef HEXGRID_H
#define HEXGRID_H

#include "raylib.h"
#include <vector>

// Hexagonal grid using axial coordinates
/// @brief Represents a hexagonal coordinate using axial coordinates (q, r).
/// The 's' coordinate can be derived from 'q' and 'r' as q + r + s = 0.
struct Hex {
    int q; ///< The 'q' axial coordinate.
    int r; ///< The 'r' axial coordinate.

    /// @brief Overloads the equality operator to compare two Hex objects.
    /// @param other The Hex object to compare with.
    /// @return True if both 'q' and 'r' coordinates are equal, false otherwise.
    bool operator==(const Hex& other) const {
        return q == other.q && r == other.r;
    }
};

/// @brief Represents a hexagonal coordinate with floating-point axial coordinates.
/// Used for intermediate calculations when converting pixel coordinates to hexagonal coordinates.
struct FractionalHex {
    float q; ///< The floating-point 'q' axial coordinate.
    float r; ///< The floating-point 'r' axial coordinate.
};

/// @brief Manages a grid of hexagonal tiles, including drawing and coordinate conversions.
class HexGrid {
public:
    /// @brief Constructs a HexGrid object.
    /// @param width The width of the hexagonal grid (number of hexes in the q-direction).
    /// @param height The height of the hexagonal grid (number of hexes in the r-direction).
    /// @param hexSize The size of each hexagon (distance from center to a vertex).
    HexGrid(int width, int height, float hexSize);

    /// @brief Draws the entire hexagonal grid, highlighting a specified hex.
    /// @param highlightedHex The Hex to be highlighted.
    void Draw(Hex highlightedHex);

    /// @brief Converts axial hexagonal coordinates to pixel coordinates.
    /// @param hex The Hex to convert.
    /// @return The Vector2 pixel coordinates of the hex's center.
    Vector2 HexToPixel(Hex hex);

    /// @brief Converts pixel coordinates to axial hexagonal coordinates.
    /// @param pixel The Vector2 pixel coordinates.
    /// @return The Hex corresponding to the given pixel coordinates.
    Hex PixelToHex(Vector2 pixel);

    /// @brief Rounds fractional hexagonal coordinates to the nearest integer hexagonal coordinates.
    /// @param frac The FractionalHex to round.
    /// @return The rounded Hex coordinates.
    Hex HexRound(FractionalHex frac);
    
    std::vector<Hex> hexes; ///< Stores all Hex objects in the grid.

private:
    int gridWidth; ///< The width of the grid in number of hexes.
    int gridHeight; ///< The height of the grid in number of hexes.
    float hexSize; ///< The size of each hex (distance from center to vertex).
};

#endif // HEXGRID_H
