#ifndef RENDER_WINDOW_H_
#define RENDER_WINDOW_H_

#include <windows.h>
#include <string>

class RenderWindow {
public:
  RenderWindow();
  ~RenderWindow();

  HWND GetHandle() const;
  const std::string& GetCaption() const;
  int GetWidth() const;
  int GetHeight() const;
  int GetLeft() const;
  int GetTop() const;

  void SetCaption(const char* caption);
  void SetWidth(const int width);
  void SetHeight(const int height);
  void SetSize(const int width, const int height);
  void SetPosition(const int left, const int top);

  void Initialize(int width, int height, int left, int top, const char* pCaption);
  void Shutdown();
  void Paint();

  //int GetSwapChain();
  //void SetSwapChain( int swapchain );
private:
  void UpdateWindowState();

private:
  HWND hwnd_;
  std::string caption_;
  int width_;
  int height_;
  int left_;
  int top_;
  //int m_iSwapChain;
  DWORD style_;
};

inline HWND RenderWindow::GetHandle() const {
  return hwnd_;
}

inline const std::string& RenderWindow::GetCaption() const {
  return caption_;
}

#endif
