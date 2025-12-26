#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "GFX_manager.h"
#include "defines.h"
#include "font_handler.h"
#include "structs.h"
#include <vector>

class Debugger {
private:
  GFX_Manager *gfxManager;
  FontHandler *fontHandler;
  std::vector<DebugData> debugData;

  float debugUpdateTimer;
  double displayRenderTime;
  double displayLogicTime;
  double displayVisTime;
  double displayRamUsage;

  const char *MouseMaskToString(mouseMask::id m);
  const char *TileToString(tile::id t); // Helper if needed, but RenderState has string representations or I can use existing helpers?
  // RenderState has mouseTileType (id). Game used worldState.hexGrid.TileToString(rs.mouseTileType).
  // HexGrid is not available here unless I include hex_tile_grid.h.
  // Structs.h includes hex_tile_grid.h so tile::id is available.
  // TileToString is in HexGrid. I don't want to couple Debugger to HexGrid just for string conversion.
  // But RenderState stores IDs.
  // Maybe I should store strings in RenderState?
  // Game.cpp stored strings for player state, but IDs for tiles.
  // Game.cpp used `worldState.hexGrid.TileToString(...)`.
  // If I want to move this to Debugger, Debugger needs access to TileToString.
  // TileToString is const method in HexGrid.
  // I can duplicate it or make it static in HexGrid or global.
  // Or pass HexGrid* to Debugger?
  // Or pass strings in RenderState.
  
  // Let's modify RenderState in structs.h to include strings for tiles?
  // Or just accept that Debugger needs to include hex_tile_grid.h (it does via structs.h) and maybe just replicate the string function or make it static.
  // `TileToString` is in `HexGrid`.
  // I'll stick to what is simplest: Add `HexGrid* hexGrid` to Debugger or pass it in Update?
  // But `HexGrid` is part of `WorldState`. `Debugger` runs on logic thread which has access to `WorldState`.
  // `RenderState` is what is passed to `Update` though?
  // Actually, `Game::RunLogic` calls `Debugger::Update`. `RunLogic` has access to `worldState`.
  // So I can pass `worldState` or `HexGrid` to `Debugger::Update`.
  // But `Debugger` should ideally work with `RenderState` (snapshot).
  // In `Game::RunLogic`, `rs` (RenderState) is populated.
  // `Debugger` uses `rs`.
  
  // I'll make TileToString a private helper in Debugger that replicates the switch case. It's safe.

public:
  Debugger();
  void SetManagers(GFX_Manager *gfx, FontHandler *font);
  void Update(const RenderState &rs, float dt, double logicTime,
              double renderTime);
};

#endif // !DEBUGGER_H
