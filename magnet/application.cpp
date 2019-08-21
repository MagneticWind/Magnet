#include <stdio.h>

#include "render\render_manager.h"
#include "render\resource_manager.h"
#include "scene\input_manager.h"
#include "scene\scene_manager.h"
#include "scene\ientity.h"

#include "application.h"
#include "render_window.h"
#include "task_manager.h"

Application* Application::instance_ = nullptr;

Application::Application() {
  render_window_ = nullptr;
  last_frame_time_lapse_ = 0.f;

  //m_bCallbacksSet = false;

  move_forward_ = 0;
  move_left_ = 0;

}

Application::~Application() {
}

Application* Application::GetInstance() {
  return instance_;
}

bool Application::Exist() {
  return instance_ != 0;
}

void Application::Initialize() {
  instance_ = new Application();
}

void Application::GetCurrentMousePosition(int* position_x, int* position_y) {
  POINT ptCurMousePos;
  GetCursorPos(&ptCurMousePos);
  *position_x = ptCurMousePos.x;
  *position_y = ptCurMousePos.y;
}

void Application::InitializeWindow(int width, int height, int left, int top,
  const char* caption) {
  if (instance_) {
    render_window_ = std::make_unique<RenderWindow>();
    render_window_->Initialize(width, height, left, top, caption);
  }
}

void Application::DestoryWindow() {
  if (render_window_) {
    render_window_->Shutdown();
  }
}

void Application::InitializeSystem(bool console) {
  enable_console_ = console;
  InitializeSingletons();

  TaskManager::GetInstance()->BeginThreads(3);

  // a dedicated thread for rendering
  Task task;
  task.func = std::bind(&magnet::render::RenderManager::Render, magnet::render::RenderManager::GetInstance());
  TaskManager::GetInstance()->EnqueueTask(task);

  DistributeTasks();
}

void Application::DestroySystem() {
  TerminateSingletons();
}

void Application::Terminate() {
  delete instance_;
  instance_ = nullptr;
}

// main thread update function
void Application::Update() {
  //timer_.Start();

  //if (last_frame_time_lapse_ > 0.f) {
  //  printf("<LOG>: Application::Update...framerate: %f fps. \n",
  //    1.f / last_frame_time_lapse_);
  //}

  auto render_manager = magnet::render::RenderManager::GetInstance();

  if (!TaskManager::GetInstance()->HasTasks()) {
    // distribute update tasks for the next frame
    // TODO: not all tasks might be finished
    
    //std::lock_guard<std::mutex> guard(
    //  render_manager->render_frame_count_mutex_);
    int render_frame_count = render_manager->GetRenderFrameCount();

    // the number is updated from main thread, no lock needed
    int update_frame_count = render_manager->GetUpdateFrameCount();

    // first frame
    if (update_frame_count == 0) {
      render_manager->IncreaseUpdateFrameCount();
      render_manager->BeginRendering();
      return;
    }

    // wait for render thread
    while (true) {
      int frame_count = render_manager->GetRenderFrameCount();
      // if render thread finishes
      if (update_frame_count == frame_count) {
        DistributeTasks();
        render_manager->IncreaseUpdateFrameCount();
        render_manager->SwapDoubleBuffers();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }
  else {
    // help other game threads do some work
    Task task;
    TaskManager::GetInstance()->DequeueTask(task);
    task.Execute();
    return;
  }

  //last_frame_time_lapse_ = timer_.GetElapsedSeconds();
  //timer_.Stop();
}

void Application::DistributeTasks() {
  std::vector<magnet::scene::IEntity*> * entities =
    magnet::scene::SceneManager::GetInstance()->GetEntities();
  for (auto entity : *entities) {
    Task task;
    task.func = std::bind(&magnet::scene::IEntity::Update, entity);
    TaskManager::GetInstance()->EnqueueTask(task);
  }
}

void Application::OnButtonDown(char button) {  
  magnet::scene::InputManager* instance = magnet::scene::InputManager::GetInstance();
  instance->RegisterMousePress(button);
}

void Application::OnButtonUp(char button) {
  magnet::scene::InputManager* instance = magnet::scene::InputManager::GetInstance();
  instance->RegisterMouseRelease(button);
}

void Application::OnKeyDown(WPARAM wParam) {
  magnet::scene::InputManager* instance = magnet::scene::InputManager::GetInstance();
  switch (wParam)
  {
  case 'W':
  case 'w':
    key_ = 'w';
    break;
  case 'S':
  case 's':
    key_ = 's';
    break;
  case 'A':
  case 'a':
    key_ = 'a';
    break;
  case 'D':
  case 'd':
    key_ = 'd';
    break;
  case 'Q':
  case 'q':
    key_ = 'q';
    break;
  case 'E':
  case 'e':
    key_ = 'e';
    break;
  }

  instance->RegisterKeyPress(key_);
}

void Application::OnKeyUp() {
  magnet::scene::InputManager* instance = magnet::scene::InputManager::GetInstance();
  instance->RegisterKeyRelease(key_);
}

void Application::OnMouseWheel(WPARAM wparam) {
  magnet::scene::InputManager* instance = magnet::scene::InputManager::GetInstance();
  float delta = GET_WHEEL_DELTA_WPARAM(wparam);
  instance->SetWheelDelta(delta);
}

void Application::InitializeSingletons() {
  magnet::scene::SceneManager::Initialize();
  magnet::scene::InputManager::Initialize();
  magnet::render::ResourceManager::Initialize();
  magnet::render::RenderManager::Initialize(render_window_->GetWidth(),
    render_window_->GetHeight(), render_window_->GetHandle());

  TaskManager::Initialize();
}

void Application::TerminateSingletons() {

  TaskManager::Terminate();

  magnet::render::RenderManager::Terminate();
  magnet::render::ResourceManager::Terminate();
  magnet::scene::InputManager::Terminate();
  magnet::scene::SceneManager::Terminate();
}

