#ifndef MAGNET_SCENE_ICOMPONENT_H_
#define MAGNET_SCENE_ICOMPONENT_H_

#include <list>

#include "math/transformation.h"
#include "render/surface.h"

namespace magnet {
namespace render {
class Surface;
}  // namespace render

namespace scene {
class IComponent {
 public:
  enum class ComponentType {
    MESH = 0,
    CAMERA = 1,
    LIGHT = 2,
    UI = 3
  };

  IComponent();
  virtual ~IComponent();
  virtual void Initialize() = 0;
  virtual void Update(const math::Matrix4f& local_to_world) = 0;
  virtual ComponentType GetType() const = 0;

  void SetTransformation(const math::Transformationf& transformation);
  math::Transformationf GetTransformation() const;
  void AddComponent(IComponent* component);

 protected:
  math::Transformationf transformation_;
  std::list<IComponent*> child_components_;
};

inline IComponent::IComponent() {}

inline IComponent::~IComponent() {
  for (auto component : child_components_) delete component;
}

inline void IComponent::SetTransformation(const math::Transformationf& transformation) {
  transformation_ = transformation;
}

inline math::Transformationf IComponent::GetTransformation() const {
  return transformation_;
}

inline void IComponent::AddComponent(IComponent* component) {
  child_components_.push_back(component);
}
}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_ICOMPONENT_H_
