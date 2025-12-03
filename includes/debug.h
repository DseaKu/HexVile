#ifndef DEBUG_H
#define DEBUG_H
class Debugger {
private:
  bool is_enabled;

public:
  void DrawDebugInformation();
  void SetDebugger(bool enable_flag);
};

#endif // !DEBUG_H
