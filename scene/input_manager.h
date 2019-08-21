#ifndef MAGNET_SCENE_INPUT_MANAGER_H_
#define MAGNET_SCENE_INPUT_MANAGER_H_

#include <map>
#include <mutex>
#include "math/Vector2.h"

namespace magnet {
namespace scene {

class InputManager {
 public:
  enum class InputState {
    INVALID = 0,
    REGISTER,
    UNREGISTERED,
    PRESSED,
    RELEASED
  };

  static void Initialize();
  static void Terminate();
  static InputManager* GetInstance();

 private:
  InputManager();
  ~InputManager();
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  static InputManager* instance_;

 public:

  void RegisterKeyPress(char key);
  void RegisterKeyRelease(char key);
  void RegisterMousePress(char button);
  void RegisterMouseRelease(char button);
  void SetWheelDelta(int delta);

  bool KeyPressed(char key) const;
  bool KeyReleased(char key) const;
  bool MousePressed(char button) const;
  bool MouseReleased(char button) const;

  math::Vector2f MousePosition() const;
  math::Vector2f MouseDelta() const;
  int GetWheelDelta() const;

  void Update();

private:
  std::map<char, InputManager::InputState> key_input_states_;
  std::map<char, InputManager::InputState> mouse_input_states_;
  math::Vector2f current_mouse_position_;
  math::Vector2f previous_mouse_position_;
  math::Vector2f delta_mouse_position_;
  int wheel_delta_;

  std::mutex input_mutex_;
};
}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_INPUT_MANAGER_H_

