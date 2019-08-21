#ifndef MAGNET_MATH_VECTOR3_H_
#define MAGNET_MATH_VECTOR3_H_

#include <cmath>

namespace magnet {
namespace math {
template <typename T>
class Vector3 {
 public:
  Vector3();
  Vector3(T t);
  Vector3(T x, T y, T z);
  Vector3(const Vector3<T>& v);

  Vector3<T>& operator = (const Vector3<T>& v);

  const Vector3<T>& operator+() const;
  Vector3<T> operator-() const;
  Vector3<T> operator+(const Vector3<T>& v) const;
  Vector3<T>& operator+=(const Vector3<T>& v);
  Vector3<T> operator-(const Vector3<T>& v) const;
  Vector3<T>& operator-=(const Vector3<T>& v);
  Vector3<T> operator/(T t) const;
  Vector3<T>& operator/=(T t);
  Vector3<T> operator*(T t) const;
  Vector3<T>& operator*=(T t);
  Vector3<T>& operator*=(const Vector3<T>& v);

  T Length() const;
  T SquareLength() const;
  void Normalize();

 public:
  T x_;
  T y_;
  T z_;
};

using Vector3d = Vector3<double>;
using Vector3f = Vector3<float>;

template <typename T>
inline Vector3<T>::Vector3() : x_(0.0), y_(0.0), z_(0.0) {}

template <typename T>
inline Vector3<T>::Vector3(T t) : x_(t), y_(t), z_(t) {}

template <typename T>
inline Vector3<T>::Vector3(T x, T y, T z) : x_(x), y_(y), z_(z) {}

template <typename T>
inline Vector3<T>::Vector3(const Vector3<T>& v) : x_(v.x_), y_(v.y_), z_(v.z_) {}

template <typename T>
inline Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v) {
  x_ = v.x_;
  y_ = v.y_;
  z_ = v.z_;
  return *this;
}

template <typename T>
inline const Vector3<T>& Vector3<T>::operator+() const {
  return *this;
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-() const {
  return Vector3<T>(-x_, -y_, -z_);
}

template <typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const {
  return Vector3<T>(x_ + v.x_, y_ + v.y_, z_ + v.z_);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v) {
  x_ += v.x_;
  y_ += v.y_;
  z_ += v.z_;
  return *this;
}

template <typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const {
  return Vector3<T>(x_ - v.x_, y_ - v.y_, z_ - v.z_);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v) {
  x_ -= v.x_;
  y_ -= v.y_;
  z_ -= v.z_;
  return *this;
}

template <typename T>
inline Vector3<T> Vector3<T>::operator*(T t) const {
  return Vector3<T>(x_ * t, y_ * t, z_ * t);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator*=(T t) {
  x_ *= t;
  y_ *= t;
  z_ *= t;
  return *this;
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& v) {
  x_ *= v.x_;
  y_ *= v.y_;
  z_ *= v.z_;
  return *this;
}

//------------------------------------------------------------------
// unary operator, can't do f*v, but can do v*f
//	friend Vector3 operator*(const Vector3& v, const float f);
//	friend Vector3 operator*(const float f, const Vector3& v) const;
//	Vector3 operator*(const Vector3& v, const float f) const {				// binary operator
//		return Vector3(x * f, y * f, z * f);
//	}
//	Vector3 operator*(const float f, const Vector3& v) const {				// binary operator
//		return Vector3(x * f, y * f, z * f);
//	}

template <typename T>
inline Vector3<T> Vector3<T>::operator/(T t) const {
  return Vector3<T>(x_ / t, y_ / t, z_ / t);
}

template <typename T>
inline Vector3<T>& Vector3<T>::operator/=(T t) {
  x_ /= t;
  y_ /= t;
  z_ /= t;
  return *this;
}

template <typename T>
inline T Vector3<T>::Length() const {
  return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

template <typename T>
inline T Vector3<T>::SquareLength() const {
  return x_ * x_ + y_ * y_ + z_ * z_;
}

template <typename T>
inline void Vector3<T>::Normalize() {
  float r = Length();
  x_ /= r;
  y_ /= r;
  z_ /= r;
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T>& v) {
  T r = v.Length();
  return Vector3<T>(v.x_ / r, v.y_ / r, v.z_ / r);
}

//------------------------------------------------------------------
// one way to define binary operator is to use inline
/*
inline Vector3 operator*(const Vector3& v, const float f) {		// binary operator
return Vector3(v.x * f, v.y * f, v.z * f);
}

inline Vector3 operator*(const float f, const Vector3& v) {		// binary operator
return Vector3(v.x * f, v.y * f, v.z * f);
}
*/

template <typename T>
inline T Dot3(const Vector3<T>& v1, const Vector3<T>& v2) {
  return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}

// cross product respects both Left handed system and right handed system
template <typename T>
inline Vector3<T> Cross3(const Vector3<T>& v1, const Vector3<T>& v2) {
  return Vector3<T>(v1.y_ * v2.z_ - v1.z_ * v2.y_,
    v1.z_ * v2.x_ - v1.x_ * v2.z_,
    v1.x_ * v2.y_ - v1.y_ * v2.x_);
}

}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_VECTOR3_H_
