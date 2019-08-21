#include "component_factory.h"

namespace magnet {
namespace scene {

bool ComponentFactory::RegisterComponent(std::string type,
  ComponentCreator* creator) {
  if (type_to_creator_.count(type) > 0) {
    return false;
  }
  type_to_creator_[type] = creator;
  return true;
}

IComponent* ComponentFactory::CreateComponent(const std::string& type,
  const std::string& name) {
  auto iter = type_to_creator_.find(type);
  if (iter == type_to_creator_.end()) {
    return nullptr;
  }
  else {
    return iter->second->Create(name);
  }
}

ComponentFactory* ComponentFactory::GetInstance() {
  static ComponentFactory factory;
  return &factory;
}
}  // namespace scene
}  // namespace magnet
