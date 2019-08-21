#ifndef MAGNET_SCENE_NORMAL_ENTITY_H_
#define MAGNET_SCENE_NORMAL_ENTITY_H_

#include "ientity.h"
#include "entity_factory.h"

namespace magnet {
namespace scene {
class NormalEntity : public IEntity {
 public:
  explicit NormalEntity(const std::string& name);
  ~NormalEntity() override;

  void Initialize() override;
  void Update() override;
};
REGISTER_ENTITY(NormalEntity);

}  // namespace scene
}  // namespace magnet

#endif  // MAGNET_SCENE_NORMAL_ENTITY_H_
