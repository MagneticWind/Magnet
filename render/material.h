#ifndef MAGNET_RENDER_MATERIAL_H_
#define MAGNET_RENDER_MATERIAL_H_

#include "Texture.h"

#include "math\vector4.h"

#define MAX_TEXTURES_COUNT 8

namespace magnet {
namespace render {

enum MaterialType
{
  MATERIAL_NORMAL,
  MATERIAL_SKY,
  MATERIAL_TRANSPARENT,
  MAX_TYPE_COUNT
};

enum MaterialTech {
  COLOR_MAP = 1,
  NORMAL_MAP = 2,
  LIGHT_MAP = 4,
  SHADOW_CASTER = 8,
  SHADOW_RECEIVER = 16,
  PROBE_VOLUME = 32,
  SKY_SH = 64
};

class Material {
 public:
   Material() {}
  explicit Material(unsigned char tech) {
    //tech_ = 
  }

  void SetAmbient(const math::Vector4f& ambient);
  void SetDiffuse(const math::Vector4f& diffuse);
  void SetSpecular(const math::Vector4f& specular);
  void SetEmission(const math::Vector4f& emission);
  void SetExponent(float fExponent);

  void GetAmbient(math::Vector4f* result) const;
  void GetDiffuse(math::Vector4f* result) const;
  void GetSpecular(math::Vector4f* result) const;
  void GetEmission(math::Vector4f* result) const;
  float GetExponent() const;

  const Texture* GetTexture(int index) const;
  void AddTexture(Texture* texture);
  int GetTexturesCount() const;

  void GetTechString(char* tech) const {
    strcpy(tech, "");

    if (tech_ & COLOR_MAP) {
      strcat(tech, "_c");
    }
    if (tech_ & NORMAL_MAP) {
      strcat(tech, "_n");
    }
    if (tech_ & LIGHT_MAP) {
      strcat(tech, "_l");
    }
    if (tech_ & SHADOW_RECEIVER) {
      strcat(tech, "_sr");
    }
    if (tech_ & PROBE_VOLUME) {
      strcat(tech, "_p");
    }
    if (tech_ & SKY_SH) {
      strcat(tech, "_sh");
    }
  }

 private:
  std::string name_;

  Texture* textures_[MAX_TEXTURES_COUNT];
  int textures_count_;

  math::Vector4f ambient_;
  math::Vector4f diffuse_;
  math::Vector4f specular_;
  math::Vector4f emission_;
  float exponent_;

  unsigned char tech_char_;
  MaterialTech tech_;
};

inline void Material::GetAmbient(math::Vector4f* result) const {
  *result = ambient_;
}

inline void Material::GetDiffuse(math::Vector4f* result) const {
  *result = diffuse_;
}

inline void Material::GetSpecular(math::Vector4f* result) const {
  *result = specular_;
}

inline void Material::GetEmission(math::Vector4f* result) const {
  *result = emission_;
}

inline float Material::GetExponent() const {
  return exponent_;
}

inline void Material::SetAmbient(const math::Vector4f& ambient) {
  ambient_ = ambient;
}

inline void Material::SetDiffuse(const math::Vector4f& diffuse) {
  diffuse_ = diffuse;
}

inline void Material::SetSpecular(const math::Vector4f& specular) {
  specular_ = specular;
}

inline void Material::SetEmission(const math::Vector4f& emission) {
  emission_ = emission;
}

inline void Material::SetExponent(float exponent) {
  exponent_ = exponent;
}

inline void Material::AddTexture(Texture* texture) {
  textures_[textures_count_++] = texture;
}

inline int Material::GetTexturesCount() const {
  return textures_count_;
}

inline const Texture* Material::GetTexture(int index) const {
  return textures_[index];
}
}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_MATERIAL_H_
