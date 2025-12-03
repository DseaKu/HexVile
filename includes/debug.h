#ifndef DEBUG_H
#define DEBUG_H

#include "tile_map.h"

class Debugger {
private:
  bool is_enabled;

public:
  void DrawDebugInformation(HexCoords selected_tile);
  void SetDebugger(bool enable_flag);
};

#endif // !DEBUG_H
