#ifndef UI_HANDLER_H
#define UI_HANDLER_H

class UIHandler {
private:
  float scale;

public:
  UIHandler();
  void Draw();
  void Init();
  void SetScale(float scale);
};
#endif // !UI_HANDLER_H
