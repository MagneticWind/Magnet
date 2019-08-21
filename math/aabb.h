#ifndef MAGNET_MATH_AABB_H_
#define MAGNET_MATH_AABB_H_

#include "vector3.h"

namespace magnet {
namespace math {
template <typename T>
class AABB {
 public:
  AABB();
  AABB(const Vector3<T>& min, const Vector3<T>& max);

  const Vector3<T>& GetMinPoint() const;
  const Vector3<T>& GetMaxPoint() const;
  T GetRadius() const;
  Vector3<T> GetCenter() const;

  Vector3<T> ComputePositiveVertex(const Vector3<T>& normal) const;
  Vector3<T> ComputeNegativeVertex(const Vector3<T>& normal) const;

  static AABB<T> CombineBBox(const AABB& bbox1, const AABB& bbox2);

  void Update(const Vector3<T>& point);

 private:
  Vector3<T> min_;
  Vector3<T> max_;
};

using AABBf = AABB<float>;
using AABBd = AABB<double>;

template <typename T>
AABB<T>::AABB() : min_(10000.0), max_(-10000.0) {}

template <typename T>
AABB<T>::AABB(const Vector3<T>& min, const Vector3<T>& max) :
  min_(min), max_(max) {}

template <typename T>
AABB<T> AABB<T>::CombineBBox(const AABB<T> &bbox1, const AABB<T> &bbox2) {
  Vector3<T> min;
  Vector3<T> max;

  // the reason to use const reference here is to eliminate the temp object,
  // as well as decrease the getter functions calls
  const Vector3<T>& kBBox1Min = bbox1.GetMinPoint();
  const Vector3<T>& kBBox1Max = bbox1.GetMaxPoint();
  const Vector3<T>& kBBox2Min = bbox2.GetMinPoint();
  const Vector3<T>& kBBox2Max = bbox2.GetMaxPoint();

  min.x_ = min(kBBox1Min.x, kBBox2Min.x);
  min.y_ = min(kBBox1Min.y, kBBox2Min.y);
  min.z_ = min(kBBox1Min.z, kBBox2Min.z);

  max.x_ = max(kBBox1Max.x, kBBox2Max.x);
  max.y_ = max(kBBox1Max.y, kBBox2Max.y);
  max.z_ = max(kBBox1Max.z, kBBox2Max.z);

  return AABB<T>(min, max);
}

template <typename T>
inline const Vector3<T>& AABB<T>::GetMinPoint() const {
  return min_;
}

template <typename T>
inline const Vector3<T>& AABB<T>::GetMaxPoint() const {
  return max_;
}

template <typename T>
inline T AABB<T>::GetRadius() const {
  return (max_ - min_).Length() * 0.5;
}

template <typename T>
inline Vector3<T> AABB<T>::GetCenter() const {
  return (max_ + min_) * 0.5;
}

template <typename T>
inline Vector3<T> AABB<T>::ComputePositiveVertex(
    const Vector3<T>& normal) const {
  Vector3<T> vertex = min_;
  if (normal.x_ >= 0.0)
    vertex.x_ = max_.x_;
  if (normal.y_ >= 0.0)
    vertex.y_ = max_.y_;
  if (normal.z_ >= 0.0)
    vertex.z_ = max_.z_;
  return vertex;
}

template <typename T>
inline Vector3<T> AABB<T>::ComputeNegativeVertex(
    const Vector3<T>& normal) const {
  Vector3<T> vertex = max_;
  if (normal.x_ >= 0.0)
    vertex.x_ = min_.x_;
  if (normal.y_ >= 0.0)
    vertex.y_ = min_.y_;
  if (normal.z_ >= 0.0)
    vertex.z_ = min_.z_;
  return vertex;
}

template <typename T>
inline void AABB<T>::Update(const Vector3<T>& point) {
  min_.x_ = std::fminf(min_.x_, point.x_);
  min_.y_ = std::fminf(min_.y_, point.y_);
  min_.z_ = std::fminf(min_.z_, point.z_);

  max_.x_ = std::fmaxf(max_.x_, point.x_);
  max_.y_ = std::fmaxf(max_.y_, point.y_);
  max_.z_ = std::fmaxf(max_.z_, point.z_);
}
}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_AABB_H_

