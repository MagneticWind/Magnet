#ifndef MAGNET_SCENE_LIGHT_COMPONENT_H_
#define MAGNET_SCENE_LIGTH_COMPONENT_H_

#include <string>
#include "math\vector3.h"
#include "math\matrix4.h"
#include "icomponent.h"
#include "component_factory.h"

namespace magnet {
namespace scene {
enum LightType {
  LIGHT_NONE,
  LIGHT_DIRECTIONAL,
  LIGHT_POINT,
  LIGHT_SPOT,
  LIGHT_SPHERE,
  LIGHT_SH,
  LIGHT_ENVIRONMENT
};

class LightComponent : public IComponent {
 public:
  explicit LightComponent(const std::string& name);
  ~LightComponent() override;

  void Initialize() override;
  void Update(const math::Matrix4f& local_to_world) override;
  ComponentType GetType() const override;

  void SetType(LightType type);
  void SetColor(const math::Vector3f& color);
  void SetIntensity(float intensity);
  void SetPosition(const math::Vector3f& position);
  void SetDirection(const math::Vector3f& direction);
  void SetCastShadow(bool cast_shadow);
  void SetRadius(float radius);
  void SetSpotProperty(float outer_angle, float inner_angle, float fall_off);

  const math::Vector3f& GetPosition() const;
  const math::Vector3f& GetColor() const;
  const math::Vector3f& GetDirection() const;
  float GetIntensity() const;
  float GetOuterAngle() const;
  float GetInnerAngle() const;
  float GetFallOff() const;
  float GetRadius() const;
  LightType GetLightType() const;
  bool IsCastShadow() const;

 private:
  std::string name_;
  LightType light_type_;
  math::Vector3f color_;
  math::Vector3f position_;   // point light, spot light
  math::Vector3f direction_;  // directional light
  float intensity_;
  float outer_angle_;         // spot light
  float inner_angle_;         // spot light
  float fall_off_;            // spot light
  float radius_;              // sphere light
  float influence_radius_;    // point light, sphere light
  bool cast_shadow_;
};
REGISTER_COMPONENT(LightComponent);

}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_LIGHT_COMPONENT_H_
