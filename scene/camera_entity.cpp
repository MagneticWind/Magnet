#include "camera_component.h"
#include "camera_entity.h"
#include "render\surface.h"

namespace magnet {
namespace scene {
CameraEntity::CameraEntity(const std::string& name) : IEntity(name) {}

CameraEntity::~CameraEntity() {}

void CameraEntity::Initialize() {
  camera_component_->Initialize();
  for (auto component : components_)
    component->Initialize();
}

void CameraEntity::Update() {
  std::lock_guard<std::mutex> lock(render_object_mutex_);

  camera_component_->Update(transformation_.ToMatrix());

  for (auto component : components_)
    component->Update(transformation_.ToMatrix());
}
}  // namespace scene
}  // namespace magnet