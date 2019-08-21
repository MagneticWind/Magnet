#include <string.h>

#include "render_window.h"

extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

RenderWindow::RenderWindow() {
}

RenderWindow::~RenderWindow() {
}

int RenderWindow::GetWidth() const {
  RECT rect;
  GetClientRect(hwnd_, &rect);
  return(rect.right - rect.left);
}

int RenderWindow::GetHeight() const {
  RECT rect;
  GetClientRect(hwnd_, &rect);
  return rect.bottom - rect.top;
}

int RenderWindow::GetLeft() const {
  POINT point;
  point.x = 0;
  point.y = 0;
  ClientToScreen(hwnd_, &point);
  return(point.x);
}

int RenderWindow::GetTop() const {
  POINT point;
  point.x = 0;
  point.y = 0;
  ClientToScreen(hwnd_, &point);
  return(point.y);
}

void RenderWindow::SetWidth(int width) {
  width_ = width;
  UpdateWindowState();
}

void RenderWindow::SetHeight(int height) {
  height_ = height;
  UpdateWindowState();
}

void RenderWindow::SetSize(int width, int height) {
  width_ = width;
  height_ = height;
  UpdateWindowState();
}

void RenderWindow::SetPosition(int left, int top) {
  left_ = left;
  top_ = top;
  UpdateWindowState();
}

void RenderWindow::SetCaption(const char* caption) {
  caption_ = std::string(caption);
  if (hwnd_ != nullptr)
    SetWindowText(hwnd_, caption);
}

void RenderWindow::UpdateWindowState() {
  if (hwnd_ != 0) {
    RECT ClientRect;
    ClientRect.left = 0;
    ClientRect.top = 0;
    ClientRect.right = width_;
    ClientRect.bottom = height_;

    // Adjust the window size for correct device size
    RECT WindowRect = ClientRect;
    AdjustWindowRect(&WindowRect, style_, FALSE);

    int deltaX = (WindowRect.right - ClientRect.right) / 2;
    int deltaY = (WindowRect.bottom - ClientRect.right) / 2;

    MoveWindow(hwnd_, left_ - deltaX, top_ - deltaY,
      width_ + deltaX * 2, height_ + deltaY * 2, true);
  }
}

void RenderWindow::Initialize(int iWidth, int iHeight, int iLeft, int iTop, const char* pCaption) {
  width_ = iWidth;
  height_ = iHeight;
  left_ = iLeft;
  top_ = iTop;

  SetCaption(pCaption);

  WNDCLASSEX wc;

  // Setup the window class
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = 0;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "Magnet";
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  // Register the window class
  RegisterClassEx(&wc);

  // Specify the window style
  style_ = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME;

  // Record the desired device size
  RECT rc;
  rc.top = rc.left = 0;
  rc.right = width_;
  rc.bottom = height_;

  // Adjust the window size for correct device size
  AdjustWindowRect(&rc, style_, FALSE);

  long lwidth = rc.right - rc.left;
  long lheight = rc.bottom - rc.top;

  long lleft = (long)left_;
  long ltop = (long)top_;


  // Create an instance of the window
  hwnd_ = CreateWindowEx(
    NULL,							// extended style
    "Magnet",						// class name
    caption_.c_str(),					// instance title
    style_,						// window style
    lleft, ltop,						// initial x,y
    lwidth,							// initial width
    lheight,						// initial height
    NULL,							// handle to parent 
    NULL,							// handle to menu
    NULL,							// instance of this application
    NULL);							// extra creation parms

                        // Initially display the window
  ShowWindow(hwnd_, SW_SHOWNORMAL);
  UpdateWindow(hwnd_);
}

void RenderWindow::Shutdown() {
  if (hwnd_)
    DestroyWindow(hwnd_);
  hwnd_ = nullptr;
}

void RenderWindow::Paint() {

}
