#ifndef MAGNET_MATH_VECTOR2_H_
#define MAGNET_MATH_VECTOR2_H_

#include <cmath>

namespace magnet {
namespace math {
template <typename T>
class Vector2 {
 public:
  Vector2();
  Vector2(T x, T y);
  Vector2(const Vector2<T>& v);

  Vector2<T>& operator = (const Vector2<T>& v);

  const Vector2<T>& operator+() const;
  Vector2<T> operator-() const;
  Vector2<T> operator+(const Vector2<T>& v) const;
  Vector2<T>& operator+=(const Vector2<T>& v);
  Vector2<T> operator-(const Vector2<T>& v) const;
  Vector2<T>& operator-=(const Vector2<T>& v);
  Vector2<T> operator/(T t) const;
  Vector2<T>& operator/=(T t);
  Vector2<T> operator*(T t) const;
  Vector2<T>& operator*=(T t);

  T Length() const;
  T SquareLength() const;
  void Normalize();
  Vector2<T> Normalize(const Vector2<T>& v) const;

 public:
  T x_;
  T y_;
  T z_;
};

using Vector2d = Vector2<double>;
using Vector2f = Vector2<float>;

template <typename T>
inline Vector2<T>::Vector2() : x_(0.0), y_(0.0) {}

template <typename T>
inline Vector2<T>::Vector2(T x, T y) : x_(x), y_(y) {}

template <typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v) : x_(v.x_), y_(v.y_), z_(v.z_) {}

template <typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v) {
  x_ = v.x_;
  y_ = v.y_;
  return *this;
}

template <typename T>
inline const Vector2<T>& Vector2<T>::operator+() const {
  return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::operator-() const {
  return Vector2<T>(-x_, -y_);
}

template <typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const {
  return Vector2<T>(x_ + v.x_, y_ + v.y_);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v) {
  x_ += v.x_;
  y_ += v.y_;
  return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& v) const {
  return Vector2<T>(x_ - v.x_, y_ - v.y_);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& v) {
  x_ -= v.x_;
  y_ -= v.y_;
  return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::operator*(T t) const {
  return Vector2<T>(x_ * t, y_ * t);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator*=(T t) {
  x_ *= t;
  y_ *= t;
  return *this;
}

template <typename T>
inline Vector2<T> Vector2<T>::operator/(T t) const {
  return Vector2<T>(x_ / t, y_ / t);
}

template <typename T>
inline Vector2<T>& Vector2<T>::operator/=(T t) {
  x_ /= t;
  y_ /= t;
  return *this;
}

template <typename T>
inline T Vector2<T>::Length() const {
  return std::sqrt(x_ * x_ + y_ * y_);
}

template <typename T>
inline T Vector2<T>::SquareLength() const {
  return x_ * x_ + y_ * y_;
}

template <typename T>
inline void Vector2<T>::Normalize() {
  float r = Length();
  x_ /= r;
  y_ /= r;
}

template <typename T>
inline Vector2<T> Vector2<T>::Normalize(const Vector2<T>& v) const {
  T r = v.Length();
  return Vector2<T>(v.x_ / r, v.y_ / r);
}

template <typename T>
inline float Dot2(const Vector2<T>& v1, const Vector2<T>& v2) {
  return v1.x_ * v2.x_ + v1.y_ * v2.y_;
}

}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_VECTOR2_H_

