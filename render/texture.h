#ifndef MAGNET_RENDER_TEXTURE_H_
#define MAGNET_RENDER_TEXTURE_H_

#include <string>

namespace magnet {
namespace render {
enum TextureFormat {
  TEXTURE_FORMAT_R8G8B8A8_UINT,
  TEXTURE_FORMAT_R8G8B8A8_UNORM,
  TEXTURE_FORMAT_R32G32B32A32_FLOAT
};

enum TextureType {
  TEXTURE_TYPE_2D,
  TEXTURE_TYPE_CUBE
};

enum SamplerMode {
  SAMPLER_NOMIP_LINEAR_WRAP,
  SAMPLER_NOMIP_LINEAR_UNWRAP,
  SAMPLER_NOMIP_ANISO2X_WRAP,
  SAMPLER_NOMIP_ANISO2X_UNWRAP,
  SAMPLER_MIP_LINEAR_WRAP,
  SAMPLER_MIP_LINEAR_UNWRAP
};

// the label will be used in shader
enum TextureLabel {
  TEXTURE_LABEL_COLOR_0,
  TEXTURE_LABEL_COLOR_1,
  TEXTURE_LABEL_NORMAL,
  TEXTURE_LABEL_SPECULAR,
  TEXTURE_LABEL_EMISSIVE,
  TEXTURE_LABEL_SKY,
  TEXTURE_LABEL_SHADOW,
  TEXTURE_LABEL_DEPTH,
  TEXTURE_LABEL_FRAME,
  MAX_TEXTURE_LABEL_COUNT
};

class Texture {
 public:
  Texture(const std::string& name);
  Texture(const std::string& name, SamplerMode sampler,
    TextureLabel label, TextureFormat format, TextureType type);
  Texture(const std::string& name, int width, int height, TextureFormat format);
  ~Texture();

  void SetDimension(int width, int height, int depth = 0);
  void SetFormat(TextureFormat format);
  void SetType(TextureType type);

  TextureFormat GetFormat() const;
  TextureType GetType() const;
  SamplerMode GetSamplerMode() const;
  TextureLabel GetLabel() const;

  int GetWidth() const;
  int GetHeight() const;

  void* CreateDataBuffer();
  void DestroyDataBuffer();
  const std::string& GetName() const;
  const void* GetDataBufferPtr() const;

  bool IsLoaded()const;
  void SetLoaded(bool loaded);

private:
  std::string name_;
  std::string	label_name_; // name in shader
  int width_;
  int height_;
  TextureFormat format_;
  TextureType type_;
  SamplerMode sampler_mode_;
  TextureLabel label_;
  void* data_;
  bool loaded_;
};

inline void Texture::SetDimension(int width, int height, int depth) {
  width_ = width;
  height_ = height;
}

inline void Texture::SetFormat(TextureFormat format) {
  format_ = format;
}

inline void Texture::SetType(TextureType type) {
  type_ = type;
}

inline TextureFormat Texture::GetFormat() const {
  return format_;
}

inline TextureType Texture::GetType() const {
  return type_;
}

inline SamplerMode Texture::GetSamplerMode() const {
  return sampler_mode_;
}

inline TextureLabel Texture::GetLabel() const {
  return label_;
}

inline int Texture::GetWidth() const {
  return width_;
}

inline int Texture::GetHeight() const {
  return height_;
}
}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_TEXTURE_H_
