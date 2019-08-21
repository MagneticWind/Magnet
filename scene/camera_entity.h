#ifndef MAGNET_SCENE_CAMERA_ENTITY_H_
#define MAGNET_SCENE_CAMERA_ENTITY_H_

#include "ientity.h"
#include "entity_factory.h"

namespace magnet {
class CameraComponent;
namespace scene {
class CameraEntity : public IEntity {
 public:
  explicit CameraEntity(const std::string& name);
  ~CameraEntity() override;

  void Initialize() override;
  void Update() override;

 private:
  CameraComponent* camera_component_;
};
REGISTER_ENTITY(CameraEntity);

}  // namespace scene
}  // namespace magnet

#endif  // MAGNET_SCENE_CAMERA_ENTITY_H_
