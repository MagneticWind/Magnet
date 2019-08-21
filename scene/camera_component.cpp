#include "math\Transformation.h"
#include "render\render_manager.h"
#include "camera_component.h"
#include "input_manager.h"

namespace magnet {
namespace scene {
CameraComponent::CameraComponent(const std::string& name) {
  projection_matrix_.SetToIdentity();
  view_matrix_.SetToIdentity();

  lookat_ = math::Vector3f(0.f, 0.f, -1.f);
  up_ = math::Vector3f(0.f, 1.f, 0.f);
  forward_ = math::Normalize(lookat_ - transformation_.GetTranslation());

  aspect_ratio_ = 1.f;
  fov_ = 60.f;
  znear_ = 0.1f;
  zfar_ = 1000.f;

  name_ = name;
  camera_type_ = CameraType::FREE;

  right_ = math::Cross3(forward_, up_);

  is_dirty_ = true;
}

CameraComponent::~CameraComponent() {}

void CameraComponent::SetUp(const math::Vector3f& up) {
  is_dirty_ = true;
  up_ = up;
}

void CameraComponent::SetLookat(const math::Vector3f& lookat) {
  is_dirty_ = true;
  lookat_ = lookat;
}

void CameraComponent::SetLocalLookat(const math::Vector3f& local_lookat) {
  local_lookat_ = local_lookat;
}

void CameraComponent::SetAspectRatio(float aspect_ratio) {
  is_dirty_ = true;
  aspect_ratio_ = aspect_ratio;
}

void CameraComponent::SetFov(float fov) {
  is_dirty_ = true;
  fov_ = fov;
}

void CameraComponent::SetZnear(float znear) {
  is_dirty_ = true;
  znear_ = znear;
}

void CameraComponent::SetZfar(float zfar) {
  is_dirty_ = true;
  zfar_ = zfar;
}

void CameraComponent::GetViewProjectionMatrix(math::Matrix4f* view_matrix,
  math::Matrix4f* projection_matrix) {
  if (is_dirty_) {
    is_dirty_ = false;

    // view matrix update
    view_matrix_ = math::Matrix4f::LookAtLH(world_position_, lookat_, up_);

    // projection matrix update
    projection_matrix_ = math::Matrix4f::PerspectiveFovLH(fov_, aspect_ratio_, znear_, zfar_);
  }
  *view_matrix = view_matrix_;
  *projection_matrix = projection_matrix_;
}

void CameraComponent::Initialize() {

}

void CameraComponent::Update(const math::Matrix4f& local_to_world) {
  is_dirty_ = true;

  InputManager* input_manager = InputManager::GetInstance();

  // camera's local to world position
  {
    math::Vector3f translate = transformation_.GetTranslation();
    math::Vector4f position(translate.x_, translate.y_, translate.z_, 1.f);
    math::Vector4f transformed_position = local_to_world * position;
    world_position_ =
      math::Vector3f(transformed_position.x_, transformed_position.y_,
        transformed_position.z_);

    // forward and right change when position changes
    forward_ = math::Normalize(lookat_ - world_position_);

    // override to top view camera
    if (camera_type_ == CameraType::TOP_VIEW_FIXED) {
      world_position_.y_ = position.y_;
      forward_ = math::Vector3f(0, -1, 0);
      lookat_ = world_position_ + forward_;
      // TODO(hong.yuan): change up, corresponding to lane vector
    }
    else if (camera_type_ == CameraType::TOP_VIEW) {
      forward_ = math::Vector3f(0, -1, 0);
    }
    else if (camera_type_ == CameraType::THIRD_PERSON_FIXED) {
      math::Vector4f look_at_homo(local_lookat_.x_, local_lookat_.y_, local_lookat_.z_, 1.f);
      math::Vector4f entity_world_position = local_to_world * look_at_homo;
      // QVector4D entity_world_position(local_to_world.column(3));
      lookat_ = math::Vector3f(entity_world_position.x_, entity_world_position.y_,
        entity_world_position.z_);
      forward_ = math::Normalize(lookat_ - world_position_);
    }

    right_ = math::Cross3(forward_, up_);
  }

  // camera movement according to keyboard input
  math::Vector3f translation;
  if (camera_type_ == CameraType::FREE) {
    if (input_manager->KeyPressed('w')) {
      translation += forward_;
    }

    if (input_manager->KeyPressed('s')) {
      translation -= forward_;
    }

    if (input_manager->KeyPressed('a')) {
      translation -= right_;
    }

    if (input_manager->KeyPressed('d')) {
      translation += right_;
    }

    if (input_manager->KeyPressed('q')) {
      translation -= up_;
    }

    if (input_manager->KeyPressed('e')) {
      translation += up_;
    }

    static const float kSpeed = 0.1f;
    transformation_.Translate(translation * kSpeed);
    math::Vector3f position = world_position_ + translation * kSpeed;
    lookat_ = position + forward_;
    world_position_ = position;
  }
  else if (camera_type_ == CameraType::TOP_VIEW) {
    if (input_manager->KeyPressed('w')) {
      translation += up_;
    }

    if (input_manager->KeyPressed('s')) {
      translation -= up_;
    }

    if (input_manager->KeyPressed('a')) {
      translation -= right_;
    }

    if (input_manager->KeyPressed('d')) {
      translation += right_;
    }

    static const float kSpeed = 1.f;
    transformation_.Translate(translation * kSpeed);
    math::Vector3f position = world_position_ + translation * kSpeed;
    lookat_ = position + forward_;
    world_position_ = position;
  }

  // camera rotation
  if (camera_type_ != CameraType::THIRD_PERSON_FIXED &&
    camera_type_ != CameraType::TOP_VIEW_FIXED &&
    camera_type_ != CameraType::TOP_VIEW &&
    (input_manager->MousePressed('r'))) {
    static const float kRotationSpeed = 0.2f;

    math::Matrix4f rotation;

    math::Vector2f delta = input_manager->MouseDelta() * kRotationSpeed;
    //TODO
    //rotation.rotate(-delta.x_, up_);
    //rotation.rotate(-delta.y_, right_);

    forward_ = rotation * forward_;
    lookat_ = world_position_ + forward_;
    right_ = math::Cross3(forward_, up_);
  }

  //  change fov according to mouse wheel
  float mouse_wheel = input_manager->GetWheelDelta();
  static const float kWheelSpeed = 0.01f;
  if (mouse_wheel != 0) {
    SetFov(fov_ - mouse_wheel * kWheelSpeed);
    input_manager->SetWheelDelta(0);
  }

  if (is_dirty_) {
    is_dirty_ = false;

    //  projection matrix update
    projection_matrix_ = math::Matrix4f::PerspectiveFovLH(fov_, aspect_ratio_, znear_, zfar_);

    // view matrix update
    view_matrix_ = math::Matrix4f::LookAtLH(world_position_, lookat_, up_);

    //
    render::RenderManager::GetInstance()->SetCameraData(view_matrix_, projection_matrix_);
  }
}

CameraComponent::CameraType CameraComponent::GetCameraType() const {
  return camera_type_;
}

void CameraComponent::SetCameraType(CameraComponent::CameraType type) {
  camera_type_ = type;
}

IComponent::ComponentType CameraComponent::GetType() const {
  return IComponent::ComponentType::CAMERA;
}

const math::Vector3f& CameraComponent::GetPosition() const {
  return world_position_;
}
}  // namespace scene
}  // namespace magnet
