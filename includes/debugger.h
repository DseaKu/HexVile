#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "GFX_manager.h"
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
  const char *TileToString(tile::id t);

public:
  Debugger();
  void SetManagers(GFX_Manager *gfx, FontHandler *font);
  void Update(const RenderState &rs, float dt, double logicTime,
              double renderTime);
};

#endif // !DEBUGGER_H
