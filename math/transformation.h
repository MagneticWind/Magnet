#ifndef MAGNET_MATH_TRANSFORMATION_H_
#define MAGNET_MATH_TRANSFORMATION_H_

#include "matrix4.h"
#include "vector3.h"
#include "quaternion.h"

namespace magnet {
namespace math {
template <typename T>
class Transformation {
 public:
  Transformation();

  void Translate(const Vector3<T>& translation);
  void Translate(T x, T y, T z);
  void Scale(const Vector3<T>& scale);
  void Scale(T x, T y, T z);
  void Scale(T scale);
  void Rotate(const Quaternion<T>& rotation);
  void Rotate(T angle, const Vector3<T>& axis);
  void Rotate(T angle, T x, T y, T z);

  const Vector3<T>& GetTranslation() const;
  const Vector3<T>& GetScale() const;
  const Quaternion<T>& GetRotation() const;

  void SetTranslation(const Vector3<T>& translation);
  void SetScale(const Vector3<T>& scale);
  void SetRotation(const Quaternion<T>& rotation);

  Matrix4<T> ToMatrix();

 private:
  Vector3<T> translation_;
  Vector3<T> scale_;
  Quaternion<T> rotation_;

  Matrix4<T> m_;

  bool is_dirty_;
};

using Transformationf = Transformation<float>;
using Transformationd = Transformation<double>;

template <typename T>
inline Transformation<T>::Transformation()
    : translation_(0.0, 0.0, 0.0),
      scale_(1.0, 1.0, 1.0),
      rotation_(1.0, 0.0, 0.0, 0.0),
      m_(),
      is_dirty_(false) {
}

template <typename T>
inline void Transformation<T>::Translate(const Vector3<T>& translation) {
  is_dirty_ = true;
  translation_ += translation;
}

template <typename T>
inline void Transformation<T>::Translate(T x, T y, T z) {
  is_dirty_ = true;
  translation_ += Vector3<T>(x, y, z);
}

template <typename T>
inline void Transformation<T>::Scale(const Vector3<T>& scale) {
  is_dirty_ = true;
  scale_ *= scale;
}

template <typename T>
inline void Transformation<T>::Scale(T scale) {
  is_dirty_ = true;
  scale_ *= scale;
}

template <typename T>
inline void Transformation<T>::Scale(T x, T y, T z) {
  is_dirty_ = true;
  scale_ *= Vector3<T>(x, y, z);
}

template <typename T>
inline void Transformation<T>::Rotate(const Quaternion<T>& rotation) {
  is_dirty_ = true;
  rotation_ = rotation * rotation_;
}

template <typename T>
inline void Transformation<T>::Rotate(T angle, const Vector3<T>& axis) {
  is_dirty_ = true;
  Rotate(Quaternion<T>::FromAxisAndAngle(axis, angle));
}

template <typename T>
inline void Transformation<T>::Rotate(T angle, T x, T y, T z) {
  is_dirty_ = true;
  Rotate(Quaternion<T>::FromAxisAndAngle(angle, Vector3f(x, y, z)));
}

template <typename T>
inline const Vector3<T>& Transformation<T>::GetTranslation() const {
  return translation_;
}

template <typename T>
inline const Vector3<T>& Transformation<T>::GetScale() const { return scale_; }

template <typename T>
inline const Quaternion<T>& Transformation<T>::GetRotation() const {
  return rotation_;
}

template <typename T>
inline void Transformation<T>::SetTranslation(const Vector3<T>& translation) {
  is_dirty_ = true;
  translation_ = translation;
}

template <typename T>
inline void Transformation<T>::SetScale(const Vector3<T>& scale) {
  is_dirty_ = true;
  scale_ = scale;
}

template <typename T>
inline void Transformation<T>::SetRotation(const Quaternion<T>& rotation) {
  is_dirty_ = true;
  rotation_ = rotation;
}

template <typename T>
inline Matrix4<T> Transformation<T>::ToMatrix() {
  if (is_dirty_) {
    is_dirty_ = false;

    m_.SetToIdentity();
    //m_.Translate(translation_);
    //m_.rotate(rotation_);
    //m_.scale(scale_);
  }

  return m_;
}
}  // namespace render
}  // namespace ui
#endif  // UI_RENDER_TRANSFORMATION_H_
