#ifndef MAGNET_MATH_VECTOR4_H_
#define MAGNET_MATH_VECTOR4_H_

#include <cmath>

namespace magnet {
namespace math {
template <typename T>
class Vector4 {
 public:
  Vector4();
  Vector4(T x, T y, T z, T w);
  Vector4(const Vector4<T>& v);

  Vector4& operator=(const Vector4<T>& v);

  const  Vector4<T>& operator+() const;
  Vector4<T> operator-() const;
  Vector4<T> operator+(const Vector4<T>& v) const;
  Vector4<T>& operator+=(const Vector4<T>& v);
  Vector4<T> operator-(const Vector4<T>& v) const;
  Vector4<T>& operator-=(const Vector4<T>& v);
  Vector4<T> operator/(T t) const;
  Vector4<T>& operator/=(T t);
  Vector4<T> operator*(T t) const;
  Vector4<T>& operator*=(T t);

  T Length() const;
  T SquareLength() const;
  void Normalize();

 public:
  T x_;
  T y_;
  T z_;
  T w_;
};

using Vector4d = Vector4<double>;
using Vector4f = Vector4<float>;

template <typename T>
inline Vector4<T>::Vector4() : x_(0.0), y_(0.0), z_(0.0), w_(0.0) {}

template <typename T>
inline Vector4<T>::Vector4(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {}

template <typename T>
inline Vector4<T>::Vector4(const Vector4<T>& v) : x_(v.x_), y_(v.y_), z_(v.z_), w_(v.w_) {}

template <typename T>
inline Vector4<T>& Vector4<T>::operator=(const Vector4<T>& v) {
  x_ = v.x_;
  y_ = v.y_;
  z_ = v.z_;
  w_ = v.w_;
  return *this;
}

template <typename T>
inline const Vector4<T>& Vector4<T>::operator+() const {
  return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::operator-() const {
  return Vector4<T>(-x_, -y_, -z_, -w_);
}

template <typename T>
inline Vector4<T> Vector4<T>::operator+(const Vector4<T>& v) const {
  return Vector4<T>(x_ + v.x_, y_ + v.y_, z_ + v.z_, w_ + v.w_);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& v) {
  x_ += v.x_;
  y_ += v.y_;
  z_ += v.z_;
  w_ += v.w_;
  return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::operator-(const Vector4<T>& v) const {
  return Vector4<T>(x_ - v.x_, y_ - v.y_, z_ - v.z_, w - v.w_);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& v) {
  x_ -= v.x_;
  y_ -= v.y_;
  z_ -= v.z_;
  w_ -= v.w_;
  return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::operator*(T t) const {
  return Vector4<T>(x_ * t, y_ * t, z_ * t, w_ * t);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator*=(T t) {
  x_ *= t;
  y_ *= t;
  z_ *= t;
  w_ *= w;
  return *this;
}

template <typename T>
inline Vector4<T> Vector4<T>::operator/(T t) const {
  return Vector4<T>(x_ / t, y_ / t, z_ / t, w_ / t);
}

template <typename T>
inline Vector4<T>& Vector4<T>::operator/=(T t) {
  x_ /= t;
  y_ /= t;
  z_ /= t;
  w_ /= t;
  return *this;
}

template <typename T>
inline T Vector4<T>::Length() const {
  return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
}

template <typename T>
inline T Vector4<T>::SquareLength() const {
  return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
}

template <typename T>
inline void Vector4<T>::Normalize() {
  float r = Length();
  x_ /= r;
  y_ /= r;
  z_ /= r;
  w_ /= r;
}

template <typename T>
inline Vector4<T> Normalize(const Vector4<T>& v) {
  T r = v.Length();
  return Vector4<T>(v.x_ / r, v.y_ / r, v.z_ / r, v.w_ / r);
}

template <typename T>
inline float Dot4(const Vector4<T>& v1, const Vector4<T>& v2) {
  return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_ + v1.w_ * v2.w_;
}

}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_VECTOR4_H_
