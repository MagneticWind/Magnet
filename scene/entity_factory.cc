// Copyright 2019 Inceptio Technology. All Rights Reserved.
// Authors:
//    Hong Yuan (hong.yuan@inceptioglobal.ai)

#include "ui/render/entity_factory.h"

namespace ui {
namespace render {

bool EntityFactory::RegisterEntity(std::string type, EntityCreator* creator) {
  if (type_to_creator_.count(type) > 0) {
    return false;
  }
  type_to_creator_[type] = creator;
  return true;
}

IEntity* EntityFactory::CreateEntity(const std::string& type,
                                     const std::string& name) {
  auto iter = type_to_creator_.find(type);
  if (iter == type_to_creator_.end()) {
    return nullptr;
  } else {
    return iter->second->Create(name);
  }
}

EntityFactory* EntityFactory::GetInstance() {
  static EntityFactory factory;
  return &factory;
}
}  // namespace render
}  // namespace ui
