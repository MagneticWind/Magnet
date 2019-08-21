#include "light_component.h"

namespace magnet {
namespace scene {

LightComponent::LightComponent(const std::string& name) : name_(name) {

}

LightComponent::~LightComponent() {

}

void LightComponent::Initialize() {

}

void LightComponent::Update(const math::Matrix4f& local_to_world) {

}

IComponent::ComponentType LightComponent::GetType() const {
  return IComponent::ComponentType::LIGHT;
}

void LightComponent::SetType(LightType type) {
  light_type_ = type;
}

void LightComponent::SetColor(const math::Vector3f& color) {
  color_ = color;
}

void LightComponent::SetIntensity(float intensity) {
  intensity_ = intensity;
}

void LightComponent::SetPosition(const math::Vector3f& position) {
  position_ = position;
}

void LightComponent::SetDirection(const math::Vector3f& direction) {
  direction_ = direction;
}

void LightComponent::SetCastShadow(bool cast_shadow) {
  cast_shadow_ = cast_shadow;
}

void LightComponent::SetRadius(float radius) {
  radius_ = radius;
}

void LightComponent::SetSpotProperty(float outer_angle, float inner_angle, float fall_off) {
  outer_angle_ = outer_angle;
  inner_angle_ = inner_angle;
  fall_off_ = fall_off;
}

const math::Vector3f& LightComponent::GetPosition() const {
  return position_;
}

const math::Vector3f& LightComponent::GetColor() const {
  return color_;
}

const math::Vector3f& LightComponent::GetDirection() const {
  return direction_;
}

float LightComponent::GetIntensity() const {
  return intensity_;
}

float LightComponent::GetOuterAngle() const {
  return outer_angle_;
}

float LightComponent::GetInnerAngle() const {
  return inner_angle_;
}

float LightComponent::GetFallOff() const {
  return fall_off_;
}

float LightComponent::GetRadius() const {
  return radius_;
}

LightType LightComponent::GetLightType() const {
  return light_type_;
}

bool LightComponent::IsCastShadow() const {
  return cast_shadow_;
}

}  // namespace scene
}  // namespace magnet