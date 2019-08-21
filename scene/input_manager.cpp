#include <Windows.h>
#include "input_manager.h"

namespace magnet {
namespace scene {

InputManager* InputManager::instance_ = nullptr;

InputManager::InputManager() : wheel_delta_(0) {}

InputManager::~InputManager() {}

void InputManager::Initialize() {
  instance_ = new InputManager();
}

void InputManager::Terminate() {
  delete instance_;
}

InputManager* InputManager::GetInstance() {
  return instance_;
}

void InputManager::RegisterKeyPress(char key) {
  auto it = key_input_states_.find(key);
  if (it == key_input_states_.end()) {
    key_input_states_.insert(
      std::pair<int, InputManager::InputState>(key, InputState::REGISTER));
  }
  else {
    key_input_states_[key] = InputState::REGISTER;
  }
}

void InputManager::RegisterKeyRelease(char key) {
  auto it = key_input_states_.find(key);
  if (it != key_input_states_.end()) {
    key_input_states_[key] = InputState::UNREGISTERED;
  }
}

void InputManager::RegisterMousePress(char button) {
  auto it = mouse_input_states_.find(button);
  if (it == mouse_input_states_.end()) {
    mouse_input_states_.insert(
      std::pair<int, InputManager::InputState>(
        button, InputState::REGISTER));
  }
  else {
    mouse_input_states_[button] = InputState::REGISTER;
  }
}

void InputManager::RegisterMouseRelease(char button) {
  auto it = mouse_input_states_.find(button);
  if (it != mouse_input_states_.end()) {
    mouse_input_states_[button] = InputState::UNREGISTERED;
  }
}

bool InputManager::KeyPressed(char key) const {
  auto it = key_input_states_.find(key);
  if (it != key_input_states_.end()) {
    return it->second == InputState::REGISTER;
  }
  else {
    return false;
  }
}

bool InputManager::KeyReleased(char key) const {
  auto it = key_input_states_.find(key);
  if (it != key_input_states_.end()) {
    return it->second == InputState::UNREGISTERED;
  }
  else {
    return true;
  }
}

bool InputManager::MousePressed(char button) const {
  auto it = mouse_input_states_.find(button);
  if (it != mouse_input_states_.end()) {
    return it->second == InputState::REGISTER;
  }
  else {
    return false;
  }
}

bool InputManager::MouseReleased(char button) const {
  auto it = mouse_input_states_.find(button);
  if (it != mouse_input_states_.end()) {
    return it->second == InputState::UNREGISTERED;
  }
  else {
    return true;
  }
}

math::Vector2f InputManager::MousePosition() const { return current_mouse_position_; }

math::Vector2f InputManager::MouseDelta() const { return delta_mouse_position_; }

int InputManager::GetWheelDelta() const { return wheel_delta_; }

void InputManager::SetWheelDelta(int delta) { wheel_delta_ = delta; }

void InputManager::Update() {
  previous_mouse_position_ = current_mouse_position_;
  POINT mouse_position;
  GetCursorPos(&mouse_position);
  current_mouse_position_ = math::Vector2f(mouse_position.x, mouse_position.y);
  delta_mouse_position_ = current_mouse_position_ - previous_mouse_position_;
}
}  // namespace scene
}  // namespace magnet
