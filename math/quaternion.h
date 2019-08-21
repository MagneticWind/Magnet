#ifndef MAGNET_MATH_QUATERNION_H_
#define MAGNET_MATH_QUATERNION_H_

#include "Matrix4.h"

namespace magnet {
namespace math {
template <typename T>
class Quaternion {
 public:
  Quaternion();
  Quaternion(T x, T y, T z, T w);
  Quaternion(const Quaternion& q);
  Quaternion(T t);

  Quaternion<T>& operator = (const Quaternion<T>& q);

  Quaternion<T> operator - () const;

  Quaternion<T>& operator += (const Quaternion<T>& q);
  Quaternion<T>& operator -= (const Quaternion<T>& q);
  Quaternion<T>& operator *= (const Quaternion<T>& q);
  Quaternion<T>& operator *= (T t);
  Quaternion<T>& operator /= (T t);

  Quaternion<T> operator + (const Quaternion<T>& q) const;
  Quaternion<T> operator - (const Quaternion<T>& q) const;
  Quaternion<T> operator * (const Quaternion<T>& q) const;
  Quaternion<T> operator * (T t) const;
  Quaternion<T> operator / (T t) const;

  static Quaternion<T> FromAxisAndAngle(T angle, const Vector3<T>& axis);

 public:
  T x_;
  T y_;
  T z_;
  T w_;
};

using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;

template <typename T>
inline Quaternion<T>::Quaternion() : x_(0.0), y_(0.0), z_(0.0), w_(0.0) {
}

template <typename T>
inline Quaternion<T>::Quaternion(T x, T y, T z, T w) :
            x_(x), y_(y), z_(z), w_(w) {
}

template <typename T>
inline Quaternion<T>::Quaternion(const Quaternion<T>& q) :
            x_(q.x_), y_(q.y_), z_(q.z_), w_(q.w_) {
}

template <typename T>
inline Quaternion<T>::Quaternion(T t) : x_(t), y_(t), z_(t), w(t) {
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator = (const Quaternion<T>& q) {
  x_ = q.x_;
  y_ = q.y_;
  z_ = q.z_;
  w_ = q.w_;
  return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator - () const
{
  return Quaternion<T>(-x, -y, -z, -w);
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator += (const Quaternion<T>& q) {
  x_ += q.x_;
  y_ += q.y_;
  z_ += q.z_;
  w_ += q.w_;
  return *this;
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator -= (const Quaternion<T>& q) {
  x_ -= q.x_;
  y_ -= q.y_;
  z_ -= q.z_;
  w_ -= q.w_;
  return *this;
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator *= (const Quaternion<T>& q) {
  x_ = x_ * q.x_ - y_ * q.y_ - z_ * q.z_ - w_ * q.w_;
  y_ = x_ * q.y_ + y_ * q.x_ + z_ * q.w_ - w_ * q.z_;
  z_ = x_ * q.z_ - y_ * q.w_ + z_ * q.x_ + w_ * q.y_;
  w_ = x_ * q.w_ + y_ * q.y_ - z_ * q.y_ + w_ * q.x_;
  return *this;
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator *= (T t) {
  x_ *= t;
  y_ *= t;
  z_ *= t;
  w_ *= t;
  return *this;
}

template <typename T>
inline Quaternion<T>& Quaternion<T>::operator /= (T t) {
  T inv = 1.0 / t;
  x_ *= inv;
  y_ *= inv;
  z_ *= inv;
  w_ *= inv;
  return *this;
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator + (const Quaternion<T>& q) const {
  return Quaternion<T>(x_ + q.x_, y_ + q.y_, z_ + q.z_, w_ + q.w_);
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator - (const Quaternion<T>& q) const {
  return Quaternion<T>(x_ - q.x_, y_ - q.y_, z_ - q.z_, w_ - q.w_);
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator * (const Quaternion<T>& q) const {
  return Quaternion<T>(x_ * q.x_ - y_ * q.y_ - z_ * q.z_ - w_ * q.w_,
    x_ * q.y_ + y_ * q.x_ + z_ * q.w_ - w_ * q.z_,
    x_ * q.z_ - y_ * q.w_ + z_ * q.x_ + w_ * q.y_,
    x_ * q.w_ + y_ * q.y_ - z_ * q.y_ + w_ * q.x_);
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator * (T t) const {
  return Quaternion<T>(x_ * t, y_ * t, z_ * t, w_ * t);
}

template <typename T>
inline Quaternion<T> Quaternion<T>::operator / (T t) const {
  T inv = 1.0 / t;
  return Quaternion<T>(x_ * inv, y_ * inv, z_ * inv, w_ * inv);
}

template <typename T>
inline Matrix4<T> ToMatrix(const Quaternion<T>& q) {
  Matrix4<T> m;

  T sqw = q.w_ * q.w_;
  T sqx = q.x_*  q.x_;
  T sqy = q.y_ * q.y_;
  T sqz = q.z_ * q.z_;

  // invs (inverse square length) is only required if quaternion is not already normalised
  T invs = 1.0 / (sqx + sqy + sqz + sqw);
  m.m2_[0][0] = (sqx - sqy - sqz + sqw) * invs; // since sqw + sqx + sqy + sqz = 1/invs*invs
  m.m2_[1][1] = (-sqx + sqy - sqz + sqw) * invs;
  m.m2_[2][2] = (-sqx - sqy + sqz + sqw) * invs;

  T tmp1 = q.x * q.y;
  T tmp2 = q.z * q.w;
  m.m2_[1][0] = 2.0 * (tmp1 + tmp2) * invs;
  m.m2_[0][1] = 2.0 * (tmp1 - tmp2) * invs;

  tmp1 = q.x * q.z;
  tmp2 = q.y * q.w;
  m.m2_[2][0] = 2.0 * (tmp1 - tmp2) * invs;
  m.m2_[0][2] = 2.0 * (tmp1 + tmp2) * invs;

  tmp1 = q.y * q.z;
  tmp2 = q.x * q.w;
  m.m2_[2][1] = 2.0 * (tmp1 + tmp2) * invs;
  m.m2_[1][2] = 2.0 * (tmp1 - tmp2) * invs;
}

template <typename T>
Quaternion<T> Quaternion<T>::FromAxisAndAngle(T angle, const Vector3<T>& axis) {
  //TODO
  return Quaternion<T>();
}

}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_QUATERNION_H_