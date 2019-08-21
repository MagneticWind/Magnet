#include <windows.h>
#include <stdio.h>
#include "application.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

bool g_bPrintDebugInfo = true;   // print debug information to console


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow) {
  // initialize singleton
  Application::Initialize();

  // initialize window and system
  Application* application = Application::GetInstance();
  application->InitializeWindow(1024, 1024, 0, 0, "magnet");
  application->InitializeSystem(g_bPrintDebugInfo);

  // create the console window
  if (g_bPrintDebugInfo) {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
  }

  MSG msg;
  bool loop = true; 
  while (loop) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        loop = false;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    application->Update();
  }

  application->DestroySystem();
  application->DestoryWindow();
  Application::Terminate();

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  Application* application = Application::GetInstance();
  switch (uMsg)
  {
  case WM_CREATE:
  {
    return 0;
  }
  case WM_ACTIVATE:
  {
    return 0;                   // Return To The Message Loop
  }
  case WM_SYSCOMMAND:
  {
    switch (wParam)             // Check System Calls
    {
    case SC_SCREENSAVE:     // Screensaver Trying To Start?
    case SC_MONITORPOWER:   // Monitor Trying To Enter Powersave?
      return 0;               // Prevent From Happening
    }
    break;
  }
  case WM_CLOSE:                      // Did We Receive A Close Message?
  {
    PostQuitMessage(0);         // Send A Quit Message
    return 0;
  }
  case WM_DESTROY:
  {
    PostQuitMessage(0);         // Send A Quit Message
    return 0;                   // Jump Back
  }
  case WM_SIZE:
  {
    //resizeDXWindow( LOWORD(lParam),HIWORD(lParam));		// LoWord=Width, HiWord=Height
    return 0;						// Jump Back
  }
  case WM_KEYDOWN:
  {
    application->OnKeyDown(wParam);
    break;
  }
  case WM_KEYUP:
  {
    application->OnKeyUp();
    break;
  }
  case WM_LBUTTONDOWN:
  {
    application->OnButtonDown('l');
    break;
  }
  case WM_LBUTTONUP:
  {
    application->OnButtonUp('l');
    break;
  }
  case WM_RBUTTONDOWN:
  {
    application->OnButtonDown('r');
    break;
  }
  case WM_RBUTTONUP:
  {
    application->OnButtonUp('r');
    break;
  }
  case WM_MOUSEWHEEL:
  {
    application->OnMouseWheel(wParam);
    break;
  }
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}