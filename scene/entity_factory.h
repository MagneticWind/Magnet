#ifndef MAGNET_SCENE_ENTITY_FACTORY_H_
#define MAGNET_SCENE_ENTITY_FACTORY_H_

#include <map>
#include <string>

#include "ientity.h"

namespace magnet {
namespace scene {

class EntityCreator {
 public:
  EntityCreator() = default;
  EntityCreator(const EntityCreator&) = delete;
  EntityCreator& operator=(const EntityCreator&) = delete;
  virtual ~EntityCreator() = default;

  virtual IEntity* Create(const std::string& name) = 0;
};

class EntityFactory {
 public:
  bool RegisterEntity(std::string type, EntityCreator* creator);
  IEntity* CreateEntity(const std::string& type, const std::string& name);

  static EntityFactory* GetInstance();

 private:
  EntityFactory() = default;
  ~EntityFactory() {}
  EntityFactory(const EntityFactory&) = delete;
  EntityFactory& operator=(const EntityFactory&) = delete;

  std::map<std::string, EntityCreator*> type_to_creator_;
};

#define REGISTER_ENTITY(typename)                                         \
  class typename##Creator : public magnet::scene::EntityCreator {            \
   public:                                                                \
    typename##Creator() {                                                 \
      magnet::scene::EntityFactory::GetInstance()->RegisterEntity(#typename, \
                                                               this);     \
    }                                                                     \
    ~typename##Creator() = default;                                       \
    magnet::scene::IEntity* Create(const std::string& name) final {          \
      return new typename(name);                                          \
    }                                                                     \
  };                                                                      \
  static typename##Creator typename##_creator_

}  // namespace scene
}  // namespace magnet

#endif  // MAGNET_SCENE_ENTITY_FACTORY_H_
