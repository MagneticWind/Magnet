#ifndef MAGNET_SCENE_IENTITY_H_
#define MAGNET_SCENE_IENTITY_H_

#include <list>
#include <memory>
#include <mutex>
#include <string>

#include "math/transformation.h"

#include "icomponent.h"

namespace magnet {
namespace scene {
class IEntity {
 public:
  explicit IEntity(const std::string& name);
  virtual ~IEntity();
  virtual void Initialize() = 0;
  virtual void Update() = 0;

  void SetTransformation(const math::Transformationf& transformation);
  const math::Transformationf& GetTransformation() const;
  std::list<IComponent*>& GetComponents();
  void AddComponent(IComponent* component);
  std::string GetName() const;

 protected:
  std::string name_;
  math::Transformationf transformation_;
  std::list<IComponent*> components_;
  std::mutex render_object_mutex_; // lock when update in render thread
};

inline IEntity::IEntity(const std::string& name) : name_(name) {}

inline IEntity::~IEntity() {
  for (auto component : components_) delete component;
}

inline std::string IEntity::GetName() const {
  return name_;
}

inline void IEntity::SetTransformation(const math::Transformationf& transformation) {
  transformation_ = transformation;
}

inline const math::Transformationf& IEntity::GetTransformation() const {
  return transformation_;
}

inline void IEntity::AddComponent(IComponent* component) {
  components_.push_back(component);
}

inline std::list<IComponent*>& IEntity::GetComponents() { return components_; }

}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_IENTITY_H_
