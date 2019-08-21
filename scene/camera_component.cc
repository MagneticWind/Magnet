// Copyright 2019 Inceptio Technology. All Rights Reserved.
// Authors:
//    Hong Yuan (hong.yuan@inceptioglobal.ai)
//    Hongjie Huang (hongjie.huang@inceptioglobal.ai)

#include "QtGui/qtransform.h"

#include "glog/logging.h"

#include "ui/render/camera_component.h"
#include "ui/render/input_manager.h"

namespace ui {
namespace render {
CameraComponent::CameraComponent(const std::string& name) {
  projection_matrix_.setToIdentity();
  view_matrix_.setToIdentity();

  lookat_ = QVector3D(0.f, 0.f, -1.f);
  up_ = QVector3D(0.f, 1.f, 0.f);
  forward_ = (lookat_ - transformation_.GetTranslation()).normalized();

  aspect_ratio_ = 1.f;
  fov_ = 60.f;
  znear_ = 0.1f;
  zfar_ = 1000.f;

  name_ = name;
  camera_type_ = CameraType::FREE;

  right_ = QVector3D::crossProduct(forward_, up_);

  rect_ = QRect(0, 0, 800, 800);

  is_dirty_ = true;
}

CameraComponent::~CameraComponent() {}

void CameraComponent::SetUp(const QVector3D& up) {
  is_dirty_ = true;
  up_ = up;
}

void CameraComponent::SetLookat(const QVector3D& lookat) {
  is_dirty_ = true;
  lookat_ = lookat;
}

void CameraComponent::SetLocalLookat(const QVector3D& local_lookat) {
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

void CameraComponent::SetRect(const QRect& rect) {
  rect_ = rect;
  SetAspectRatio(static_cast<float>(rect.width()) / rect.height());
}

void CameraComponent::GetViewProjectionMatrix(QMatrix4x4* view_matrix,
                                              QMatrix4x4* projection_matrix) {
  if (is_dirty_) {
    is_dirty_ = false;

    // view matrix update
    view_matrix_.setToIdentity();
    view_matrix_.lookAt(world_position_, lookat_, up_);

    // projection matrix update
    projection_matrix_.setToIdentity();
    projection_matrix_.perspective(fov_, aspect_ratio_, znear_, zfar_);
  }
  *view_matrix = view_matrix_;
  *projection_matrix = projection_matrix_;
}

void CameraComponent::Update(float /*delta_time*/,
                             const QMatrix4x4& local_to_world,
                             InputManager* input_manager) {
  is_dirty_ = true;

  // camera's local to world position
  {
    QVector3D translate = transformation_.GetTranslation();
    QVector4D position(translate.x(), translate.y(), translate.z(), 1.f);
    QVector4D transformed_position = local_to_world * position;
    world_position_ =
        QVector3D(transformed_position.x(), transformed_position.y(),
                  transformed_position.z());

    // forward and right change when position changes
    forward_ = (lookat_ - world_position_).normalized();

    // override to top view camera
    if (camera_type_ == CameraType::TOP_VIEW_FIXED) {
      world_position_.setY(position.y());
      forward_ = QVector3D(0, -1, 0);
      lookat_ = world_position_ + forward_;
      // TODO(hong.yuan): change up, corresponding to lane vector
    } else if (camera_type_ == CameraType::TOP_VIEW) {
      forward_ = QVector3D(0, -1, 0);
    } else if (camera_type_ == CameraType::THIRD_PERSON_FIXED) {
      QVector4D entity_world_position = local_to_world * local_lookat_;
      // QVector4D entity_world_position(local_to_world.column(3));
      lookat_ = QVector3D(entity_world_position.x(), entity_world_position.y(),
                          entity_world_position.z());
      forward_ = (lookat_ - world_position_).normalized();
    }

    right_ = QVector3D::crossProduct(forward_, up_);
  }

  // camera movement according to keyboard input
  QVector3D translation;
  if (camera_type_ == CameraType::FREE) {
    if (input_manager->KeyPressed(Qt::Key_W)) {
      translation += forward_;
    }

    if (input_manager->KeyPressed(Qt::Key_S)) {
      translation -= forward_;
    }

    if (input_manager->KeyPressed(Qt::Key_A)) {
      translation -= right_;
    }

    if (input_manager->KeyPressed(Qt::Key_D)) {
      translation += right_;
    }

    if (input_manager->KeyPressed(Qt::Key_Q)) {
      translation -= up_;
    }

    if (input_manager->KeyPressed(Qt::Key_E)) {
      translation += up_;
    }

    static const float kSpeed = 0.1f;
    transformation_.Translate(translation * kSpeed);
    QVector3D position = world_position_ + translation * kSpeed;
    lookat_ = position + forward_;
    world_position_ = position;
  } else if (camera_type_ == CameraType::TOP_VIEW) {
    if (input_manager->KeyPressed(Qt::Key_W)) {
      translation += up_;
    }

    if (input_manager->KeyPressed(Qt::Key_S)) {
      translation -= up_;
    }

    if (input_manager->KeyPressed(Qt::Key_A)) {
      translation -= right_;
    }

    if (input_manager->KeyPressed(Qt::Key_D)) {
      translation += right_;
    }

    static const float kSpeed = 1.f;
    transformation_.Translate(translation * kSpeed);
    QVector3D position = world_position_ + translation * kSpeed;
    lookat_ = position + forward_;
    world_position_ = position;
  }

  // camera rotation
  if (camera_type_ != CameraType::THIRD_PERSON_FIXED &&
      camera_type_ != CameraType::TOP_VIEW_FIXED &&
      camera_type_ != CameraType::TOP_VIEW &&
      (input_manager->MousePressed(Qt::RightButton))) {
    static const float kRotationSpeed = 0.2f;

    QMatrix4x4 rotation;

    QPoint delta = input_manager->MouseDelta() * kRotationSpeed;
    rotation.rotate(-delta.x(), up_);
    rotation.rotate(-delta.y(), right_);

    forward_ = rotation * forward_;
    lookat_ = world_position_ + forward_;
    right_ = QVector3D::crossProduct(forward_, up_);
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
    projection_matrix_.setToIdentity();
    projection_matrix_.perspective(fov_, aspect_ratio_, znear_, zfar_);

    // view matrix update
    view_matrix_.setToIdentity();
    view_matrix_.lookAt(world_position_, lookat_, up_);
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

void CameraComponent::Render(const QMatrix4x4& model_matrix,
                             const QMatrix4x4& view_matrix,
                             const QMatrix4x4& projection_matrix) {
  // not renderable
  QMatrix4x4 temp = model_matrix;
  temp = view_matrix;
  temp = projection_matrix;
}

const QVector3D& CameraComponent::GetPosition() const {
  return world_position_;
}
}  // namespace render
}  // namespace ui
