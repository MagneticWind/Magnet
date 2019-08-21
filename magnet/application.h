#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <memory>
#include <Windows.h>

#include "timer.h"

class RenderWindow;

class Application {
 private:
  Application();
  ~Application();
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

 public:
  static Application* GetInstance();
  static bool Exist();
  static void Initialize();
  static void Terminate();

  static void GetCurrentMousePosition(int* position_x, int* position_y);

  void InitializeWindow(int width, int height, int left, int top, const char* caption);
  void DestoryWindow();
  void InitializeSystem(bool bConsole);
  void DestroySystem();

  // get called each frame
  void Update();

  // handle inputs: mouse and keyboard
  void OnButtonDown(char button);
  void OnButtonUp(char button);
  void OnKeyDown(WPARAM wParam);
  void OnKeyUp();
  void OnMouseWheel(WPARAM wParam);

 private:
  void InitializeSingletons();
  void TerminateSingletons();

  void DistributeTasks();

 private:
  static Application* instance_;

  std::unique_ptr<RenderWindow> render_window_;
  bool enable_logging_;
  bool enable_console_;

  Timer timer_;
  float last_frame_time_lapse_;

  int move_forward_;
  int move_left_;

  char key_;
};



#endif