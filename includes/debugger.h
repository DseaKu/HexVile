#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "GFX_manager.h"
#include "font_handler.h"
#include "structs.h"
#include <vector>

class Debugger {
private:
  // --- Dependencies ---
  GFX_Manager *gfxManager;
  FontHandler *fontHandler;

  // --- State ---
  std::vector<DebugData> debugData;

  // --- Profiling ---
  float debugUpdateTimer;
  double displayRenderTime;
  double displayLogicTime;
  double displayVisTime;
  double displayRamUsage;

  // --- Private Helpers ---
  const char *MouseMaskToString(mouseMask::id m);
  const char *TileToString(tile::id t);

public:
  // --- Constructors ---
  Debugger();

  // --- Core Lifecycle ---
  void Update(const RenderState &rs, float dt, double logicTime,
              double renderTime);

  // --- Graphics / Backbuffer ---
  void LoadBackBuffer();

  // --- Setters ---
  void SetManagers(GFX_Manager *gfx, FontHandler *font);
};

#endif // !DEBUGGER_H
