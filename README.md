# HexVile

A 2D hexagonal tile-based exploration and resource gathering game built with C++ and [Raylib](https://www.raylib.com/).

![HexVile Gameplay](demo.gif)

## Features

- **Hexagonal Grid System**: A robust hex-based world with smooth coordinate translation and rendering.
- **Resource Gathering**: Interactive environment features like tree chopping for wood collection.
- **Dynamic Player System**: State-based animations, directional facing, and fluid movement.
- **Inventory & Toolbar**: Simple item management and active slot selection.
- **Multi-threaded Architecture**: Separation of game logic and rendering threads for optimal performance.
- **Custom Shaders**: Includes visual effects like hit-flashing for interactive objects.
- **Debug Overlay**: Real-time performance metrics (FPS, Logic/Render times, RAM usage) and grid visualization.

## Controls

- **W, A, S, D**: Move character.
- **1 - 9, 0**: Select toolbar slots.
- **Left Click**: Interact with the world (e.g., chop trees when Axe is selected).
- **I**: Toggle Inventory (Work in progress).
- **F**: Toggle Fullscreen mode.

## Prerequisites

To build and run HexVile, you need:

- A C++17 compatible compiler (GCC, Clang, or MSVC).
- [CMake](https://cmake.org/) (Version 3.10 or higher).
- OpenGL drivers.

## Building the Project

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/hex_vile_cpp.git
   cd hex_vile_cpp
   ```

2. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```

3. **Configure and build**:
   ```bash
   cmake ..
   make
   ```

4. **Run the game**:
   ```bash
   ./game
   ```

*Note: Raylib is automatically fetched and compiled via CMake's `FetchContent` module during the first build.*

## Project Structure

- `src/`: Core implementation files (`main`, `game`, `player`, `hex_tile_grid`, etc.).
- `includes/`: Header files and system defines.
- `assets/`:
    - `images/`: Texture atlases.
    - `font/`: Game fonts.
    - `shaders/`: GLSL shader files.

## Future Plans

- [ ] Rework enum system for better identification.
- [ ] Implement texture Look-Up Tables (LUT) for performance optimization.
- [ ] Expand player animations and state transitions.
- [ ] Add more resource types and craftable items.
- [ ] Enhanced UI/UX for inventory management.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details (if applicable).
