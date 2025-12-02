#ifndef DEBUG_H
#define DEBUG_H
class Debugger {
private:
  bool is_enabled;

public:
  Debugger(bool enable_flag);
  ~Debugger();
  void DrawDebugInformation();
};

#endif // !DEBUG_H
