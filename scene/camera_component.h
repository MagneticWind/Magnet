#ifndef MAGNET_SCENE_CAMERA_COMPONENT_H_
#define MAGNEt_SCENE_CAMERA_COMPONENT_H_

#include <string>

#include "math/Matrix4.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"
#include "math/Vector2.h"

#include "component_factory.h"
#include "icomponent.h"

namespace magnet {
namespace scene {
class CameraComponent : public IComponent {
 public:
  enum class CameraType {
    FREE = 0,
    FIRST_PERSON = 1,
    THIRD_PERSON = 2,
    THIRD_PERSON_FIXED = 3,
    TOP_VIEW = 4,
    TOP_VIEW_FIXED = 5
  };

  explicit CameraComponent(const std::string& name);
  ~CameraComponent() override;

  void SetUp(const math::Vector3f& up);
  void SetLookat(const math::Vector3f& lookat);
  void SetLocalLookat(const math::Vector3f& local_lookat);
  void SetAspectRatio(float aspect_ratio);
  void SetFov(float fov);
  void SetZnear(float znear);
  void SetZfar(float zfar);

  void GetViewProjectionMatrix(math::Matrix4f* view_matrix,
                               math::Matrix4f* projection_matrix);

  const math::Vector3f& GetPosition() const;

  void Initialize() override;
  void Update(const math::Matrix4f& local_to_world) override;
  IComponent::ComponentType GetType() const override;

  CameraType GetCameraType() const;
  void SetCameraType(CameraType type);

 private:
  math::Matrix4f projection_matrix_;
  math::Matrix4f view_matrix_;

  math::Vector3f forward_;
  math::Vector3f up_;
  math::Vector3f lookat_;
  math::Vector3f local_lookat_;
  math::Vector3f right_;

  math::Vector3f world_position_;

  float aspect_ratio_;
  float fov_;
  float znear_;
  float zfar_;

  math::Quaternionf quatenion_;

  std::string name_;

  CameraType camera_type_;
  bool is_dirty_;
};
REGISTER_COMPONENT(CameraComponent);

}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_CAMERA_COMPONENT_H_
