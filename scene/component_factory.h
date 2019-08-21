// Copyright 2019 Inceptio Technology. All Rights Reserved.
// Authors:
//    Hong Yuan (hong.yuan@inceptioglobal.ai)

#ifndef UI_RENDER_COMPONENT_FACTORY_H_
#define UI_RENDER_COMPONENT_FACTORY_H_

#include <map>
#include <string>

#include "icomponent.h"

namespace magnet {
namespace scene {

class ComponentCreator {
 public:
  ComponentCreator() = default;
  ComponentCreator(const ComponentCreator&) = delete;
  ComponentCreator& operator=(const ComponentCreator&) = delete;
  virtual ~ComponentCreator() = default;

  virtual IComponent* Create(const std::string& name) = 0;
};

class ComponentFactory {
 public:
  bool RegisterComponent(std::string type, ComponentCreator* creator);
  IComponent* CreateComponent(const std::string& type, const std::string& name);

  static ComponentFactory* GetInstance();

 private:
  ComponentFactory() = default;
  ~ComponentFactory() {}
  ComponentFactory(const ComponentFactory&) = delete;
  ComponentFactory& operator=(const ComponentFactory&) = delete;

  std::map<std::string, ComponentCreator*> type_to_creator_;
};

#define REGISTER_COMPONENT(typename)                                  \
  class typename##Creator : public magnet::scene::ComponentCreator {     \
   public:                                                            \
    typename##Creator() {                                             \
      magnet::scene::ComponentFactory::GetInstance()->RegisterComponent( \
          #typename, this);                                           \
    }                                                                 \
    ~typename##Creator() = default;                                   \
    magnet::scene::IComponent* Create(const std::string& name) final {   \
      return new typename(name);                                      \
    }                                                                 \
  };                                                                  \
  static typename##Creator typename##_creator_

}  // namespace scene
}  // namespace magnet

#endif  // MAGNET_SCENE_COMPONENT_FACTORY_H_
