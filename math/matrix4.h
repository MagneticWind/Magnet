#ifndef MAGNET_MATH_MATRIX4_H_
#define MAGNET_MATH_MATRIX4_H_

#include "Vector3.h"
#include "Vector4.h"

namespace magnet {
namespace math
{
template <typename T>
class Matrix4 {
 public:
  Matrix4();
  Matrix4(const T* m);
  Matrix4(const Matrix4<T>& mat);
  Matrix4(T t);
  Matrix4(T t00, T t01, T t02, T t03,
    T t10, T t11, T t12, T t13,
    T t20, T t21, T t22, T t23,
    T t30, T t31, T t32, T t33);

  Matrix4<T>& operator=(const Matrix4<T>& mat);

  Matrix4<T> operator+(const Matrix4<T>& mat) const;
  Matrix4<T> operator-(const Matrix4<T>& mat) const;
  Matrix4<T> operator*(const Matrix4<T>& mat) const;

  Matrix4<T> Transpose() const;

  void SetToIdentity();

  static Matrix4<T> LookAtLH(const Vector3<T>& position,
    const Vector3<T>& lookat, const Vector3<T>& up);
  static Matrix4<T> LookAtRH(const Vector3<T>& position,
    const Vector3<T>& lookat, const Vector3<T>& up);
  static Matrix4<T> PerspectiveFovLH(float fov, float aspect_ratio,
    float znear, float zfar);
  static Matrix4<T> OrthographicLH(float width, float height,
    float znear, float zfar);
  static Matrix4<T> OrthographicOffCenterLH(float min_x, float max_x,
    float min_y, float max_y, float min_z, float max_z);
  static Matrix4<T> RotationYawPitchRoll(float yaw, float pitch,
    float roll);

 public:
  union {
    float m2_[4][4];
    float m1_[16];
  };
};

using Matrix4f = Matrix4<float>;
using Matrix4d = Matrix4<double>;

template <typename T>
Matrix4<T>::Matrix4() {
  for (int i = 0; i < 16; ++i)
    m1_[i] = 0.0;
  m2_[0][0] = 1.0;
  m2_[1][1] = 1.0;
  m2_[2][2] = 1.0;
  m2_[3][3] = 1.0;
}

template <typename T>
Matrix4<T>::Matrix4(const T* m) {
  for (int i = 0; i < 16; ++i)
    m1_[i] = m[i];
}

template <typename T>
Matrix4<T>::Matrix4(const Matrix4<T>& mat) {
  for (int i = 0; i < 16; ++i)
    m1_[i] = mat.m1_[i];
}

template <typename T>
Matrix4<T>::Matrix4(T t) {
  for (int i = 0; i < 16; ++i)
    m1_[i] = t;
}

template <typename T>
Matrix4<T>::Matrix4(T t00, T t01, T t02, T t03,
  T t10, T t11, T t12, T t13,
  T t20, T t21, T t22, T t23,
  T t30, T t31, T t32, T t33) {
  m2_[0][0] = t00; m2_[0][1] = t01; m2_[0][2] = t02; m2_[0][3] = t03;
  m2_[1][0] = t10; m2_[1][1] = t11; m2_[1][2] = t12; m2_[1][3] = t13;
  m2_[2][0] = t20; m2_[2][1] = t21; m2_[2][2] = t22; m2_[2][3] = t23;
  m2_[3][0] = t30; m2_[3][1] = t31; m2_[3][2] = t32; m2_[3][3] = t33;
}

template <typename T>
Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& mat) {
  for (int i = 0; i < 16; ++i)
    m1_[i] = mat.m1_[i];
  return *this;
}

template <typename T>
Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& mat) const {
  return Matrix4<T>(
    m2_[0][0] + mat.m2_[0][0],
    m2_[0][1] + mat.m2_[0][1],
    m2_[0][2] + mat.m2_[0][2],
    m2_[0][3] + mat.m2_[0][3],
    m2_[1][0] + mat.m2_[1][0],
    m2_[1][1] + mat.m2_[1][1],
    m2_[1][2] + mat.m2_[1][2],
    m2_[1][3] + mat.m2_[1][3],
    m2_[2][0] + mat.m2_[2][0],
    m2_[2][1] + mat.m2_[2][1],
    m2_[2][2] + mat.m2_[2][2],
    m2_[2][3] + mat.m2_[2][3],
    m2_[3][0] + mat.m2_[3][0],
    m2_[3][1] + mat.m2_[3][1],
    m2_[3][2] + mat.m2_[3][2],
    m2_[3][3] + mat.m2_[3][3]
    );
}

template <typename T>
Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& mat) const
{
  return Matrix4<T>(
    m2_[0][0] - mat.m2_[0][0],
    m2_[0][1] - mat.m2_[0][1],
    m2_[0][2] - mat.m2_[0][2],
    m2_[0][3] - mat.m2_[0][3],
    m2_[1][0] - mat.m2_[1][0],
    m2_[1][1] - mat.m2_[1][1],
    m2_[1][2] - mat.m2_[1][2],
    m2_[1][3] - mat.m2_[1][3],
    m2_[2][0] - mat.m2_[2][0],
    m2_[2][1] - mat.m2_[2][1],
    m2_[2][2] - mat.m2_[2][2],
    m2_[2][3] - mat.m2_[2][3],
    m2_[3][0] - mat.m2_[3][0],
    m2_[3][1] - mat.m2_[3][1],
    m2_[3][2] - mat.m2_[3][2],
    m2_[3][3] - mat.m2_[3][3]
    );
}

template <typename T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& mat) const {
  Matrix4<T> temp_matrix(0.0);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k)
        temp_matrix.m2_[i][j] += m2_[i][k] * mat.m2_[k][j];
    }
  }
  return temp_matrix;
}

template <typename T>
Matrix4<T> Matrix4<T>::Transpose() const {
  Matrix4<T> temp_matrix(0.0);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      temp_matrix.m2_[i][j] = m2_[j][i];
    }
  }
  return temp_matrix;
}

template <typename T>
void Matrix4<T>::SetToIdentity() {
  for (int i = 0; i < 16; ++i)
    m1_[i] = 0.0;
  m2_[0][0] = 1.0;
  m2_[1][1] = 1.0;
  m2_[2][2] = 1.0;
  m2_[3][3] = 1.0;
}

template <typename T>
Matrix4<T> Matrix4<T>::LookAtLH(const Vector3<T>& position,
  const Vector3<T>& lookat, const Vector3<T>& up) {
  Vector3<T> z_axis = lookat - position;
  z_axis.Normalize();

  Vector3<T> x_axis;
  if (std::abs(1.0 - std::abs(z_axis.y_)) < 0.00001)
    x_axis = Vector3<T>(-1.0, 0.0, 0.0);
  else
    z_axis = Cross3(up, z_axis);

  x_axis.Normalize();
  Vector3<T> y_axis = Cross3(z_axis, x_axis);
  y_axis.Normalize();

  Matrix4<T> result;
  result.m2_[0][0] = x_axis.x_;
  result.m2_[0][1] = x_axis.y_;
  result.m2_[0][2] = x_axis.z_;
  result.m2_[0][3] = -Dot3(x_axis, position);
  result.m2_[1][0] = y_axis.x_;
  result.m2_[1][1] = y_axis.y_;
  result.m2_[1][2] = y_axis.z_;
  result.m2_[1][3] = -Dot3(y_axis, position);
  result.m2_[2][0] = z_axis.x_;
  result.m2_[2][1] = z_axis.y_;
  result.m2_[2][2] = z_axis.z_;
  result.m2_[2][3] = -Dot3(z_axis, position);
  result.m2_[3][0] = 0;
  result.m2_[3][1] = 0;
  result.m2_[3][2] = 0;
  result.m2_[3][3] = 1;

  return result;
}

template <typename T>
Matrix4<T> Matrix4<T>::LookAtRH(const Vector3<T>& position,
  const Vector3<T>& lookat, const Vector3<T>& up) {
  Vector3<T> z_axis = position - lookat;
  z_axis.Normalize();
  Vector3<T> x_axis = Cross3(up, z_axis);
  x_axis.Normalize();
  Vector3<T> y_axis = Cross3(z_axis, x_axis);
  y_axis.Normalize();

  Matrix4<T> result;
  result.m2_[0][0] = x_axis.x;
  result.m2_[0][1] = x_axis.y;
  result.m2_[0][2] = x_axis.z;
  result.m2_[0][3] = -Dot3(x_axis, position);
  result.m2_[1][0] = y_axis.x;
  result.m2_[1][1] = y_axis.y;
  result.m2_[1][2] = y_axis.z;
  result.m2_[1][3] = -Dot3(y_axis, position);
  result.m2_[2][0] = z_axis.x;
  result.m2_[2][1] = z_axis.y;
  result.m2_[2][2] = z_axis.z;
  result.m2_[2][3] = -Dot3(z_axis, position);
  result.m2_[3][0] = 0;
  result.m2_[3][1] = 0;
  result.m2_[3][2] = 0;
  result.m2_[3][3] = 1;

  return result;
}

template <typename T>
Matrix4<T> Matrix4<T>::PerspectiveFovLH(float fov, float aspect_ratio,
  float znear, float zfar) {
  float y_scale = 1.0 / std::tan(fov * 0.5);
  float x_scale = y_scale / aspect_ratio;

  Matrix4<T> result;
  result.m2_[0][0] = x_scale;
  result.m2_[0][1] = 0;
  result.m2_[0][2] = 0;
  result.m2_[0][3] = 0;
  result.m2_[1][0] = 0;
  result.m2_[1][1] = y_scale;
  result.m2_[1][2] = 0;
  result.m2_[1][3] = 0;
  result.m2_[2][0] = 0;
  result.m2_[2][1] = 0;
  result.m2_[2][2] = zfar / (zfar - znear);
  result.m2_[2][3] = -zfar * znear / (zfar - znear);
  result.m2_[3][0] = 0;
  result.m2_[3][1] = 0;
  result.m2_[3][2] = 1;
  result.m2_[3][3] = 0;

  return result;
}

template <typename T>
Matrix4<T> Matrix4<T>::OrthographicLH(float width, float height,
  float znear, float zfar) {
  Matrix4<T> result;
  result.m2_[0][0] = 2.0 / width;
  result.m2_[0][1] = 0;
  result.m2_[0][2] = 0;
  result.m2_[0][3] = 0;
  result.m2_[1][0] = 0;
  result.m2_[1][1] = 2.0 / height;
  result.m2_[1][2] = 0;
  result.m2_[1][3] = 0;
  result.m2_[2][0] = 0;
  result.m2_[2][1] = 0;
  result.m2_[2][2] = 1.0 / (zfar - znear);
  result.m2_[2][3] = -znear / (zfar - znear);
  result.m2_[3][0] = 0;
  result.m2_[3][1] = 0;
  result.m2_[3][2] = 0;
  result.m2_[3][3] = 1;

  return result;
}

template <typename T>
Matrix4<T> Matrix4<T>::OrthographicOffCenterLH(float min_x, float max_x,
  float min_y, float max_y, float min_z, float max_z) {
  Matrix4<T> result;
  mProjection.m2[0][0] = 2.0 / (max_x - min_x);
  mProjection.m2[0][1] = 0;
  mProjection.m2[0][2] = 0;
  mProjection.m2[0][3] = (min_x + max_x) / (min_x - max_x);
  mProjection.m2[1][0] = 0;
  mProjection.m2[1][1] = 2.0 / (max_y - min_y);
  mProjection.m2[1][2] = 0;
  mProjection.m2[1][3] = (min_y + max_y) / (min_y - max_y);
  mProjection.m2[2][0] = 0;
  mProjection.m2[2][1] = 0;
  mProjection.m2[2][2] = 1.0 / (max_z - min_z);
  mProjection.m2[2][3] = min_z / (min_z - max_z);
  mProjection.m2[3][0] = 0;
  mProjection.m2[3][1] = 0;
  mProjection.m2[3][2] = 0;
  mProjection.m2[3][3] = 1;

  return result;
}

template <typename T>
Matrix4<T> Matrix4<T>::RotationYawPitchRoll(float yaw, float pitch,
  float roll) {
  float sx = std::sin(pitch);
  float sy = std::sin(yaw);
  float sz = std::sin(roll);
  float cx = std::cos(pitch);
  float cy = std::cos(yaw);
  float cz = std::cos(roll);

  Matrix4<T> result;
  result.m2_[0][0] = cz * cy + sx * sy * sz;
  result.m2_[0][1] = sz * cx;
  result.m2_[0][2] = -cz * sy + sz * sx * cy;
  result.m2_[0][3] = 0.0f;

  result.m2_[1][0] = -sz * cy + cz * sx * sy;
  result.m2_[1][1] = cx * cz;
  result.m2_[1][2] = sz * sy + cz * sx * cy;
  result.m2_[1][3] = 0.0f;

  result.m2_[2][0] = cx * sy;
  result.m2_[2][1] = -sx;
  result.m2_[2][2] = cx * cy;
  result.m2_[2][3] = 0.0f;

  result.m2_[3][0] = 0.0f;
  result.m2_[3][1] = 0.0f;
  result.m2_[3][2] = 0.0f;
  mRotation.m2[3][3] = 1.0f;
}


template<typename T>
inline Vector4<T> operator*(const Matrix4<T>& mat4, const Vector4<T>& vec4)
{
  T x = mat4.m1_[0] * vec4.x_ + mat4.m1_[1] * vec4.y_ + mat4.m1_[2] * vec4.z_ + mat4.m1_[3] * vec4.w_;
  T y = mat4.m1_[4] * vec4.x_ + mat4.m1_[5] * vec4.y_ + mat4.m1_[6] * vec4.z_ + mat4.m1_[7] * vec4.w_;
  T z = mat4.m1_[8] * vec4.x_ + mat4.m1_[9] * vec4.y_ + mat4.m1_[10] * vec4.z_ + mat4.m1_[11] * vec4.w_;
  T w = mat4.m1_[12] * vec4.x_ + mat4.m1_[13] * vec4.y_ + mat4.m1_[14] * vec4.z_ + mat4.m1_[15] * vec4.w_;
  return Vector4<T>(x, y, z, w);
}

template<typename T>
inline Vector3<T> operator*(const Matrix4<T>& mat4, const Vector3<T>& vec3)
{
  Vector4<T> temp(vec3.x_, vec3.y_, vec3.z_, 1.0);
  Vector4<T> result = mat4 * temp;
  return Vector3<T>(result.x_ / result.w_, result.y_ / result.w_, result.z_ / result.w_);
}

}  // namespace math
}  // namespace magnet
#endif  // MAGNET_MATH_MATRIX4_H_
