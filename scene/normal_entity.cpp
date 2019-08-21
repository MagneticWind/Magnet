#include "normal_entity.h"
#include "render\surface.h"

namespace magnet {
namespace scene {
NormalEntity::NormalEntity(const std::string& name) : IEntity(name) {}

NormalEntity::~NormalEntity() {}

void NormalEntity::Initialize() {
  for (auto component : components_)
    component->Initialize();
}

void NormalEntity::Update() {
  std::lock_guard<std::mutex> lock(render_object_mutex_);
  for (auto component : components_)
    component->Update(transformation_.ToMatrix());
}
}  // namespace scene
}  // namespace magnet