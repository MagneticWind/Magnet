#include <string>
#include <stdlib.h>

#include "texture.h"

namespace magnet {
namespace render {
Texture::Texture(const std::string& name) : name_(name),
  sampler_mode_(SAMPLER_NOMIP_LINEAR_WRAP),
  type_(TEXTURE_TYPE_2D),
  loaded_(false),
  data_(nullptr) {}

Texture::Texture(const std::string& name, SamplerMode sampler,
  TextureLabel label, TextureFormat format, TextureType type) :
  name_(name),
  sampler_mode_(sampler),
  label_(label),
  format_(format),
  type_(type),
  loaded_(false),
  data_(nullptr) {}

Texture::Texture(const std::string& name, int width, int height,
  TextureFormat format) :
  name_(name),
  width_(width),
  height_(height),
  format_(format),
  sampler_mode_(SAMPLER_NOMIP_LINEAR_WRAP),
  type_(TEXTURE_TYPE_2D),
  loaded_(false),
  data_(nullptr) {}

Texture::~Texture() {
  if (data_) {
    free(data_);
    data_ = nullptr;
  }
}

const std::string& Texture::GetName() const {
  return name_;
}

const void* Texture::GetDataBufferPtr() const {
  return data_;
}

bool Texture::IsLoaded() const {
  return loaded_;
}

void Texture::SetLoaded(bool loaded) {
  loaded_ = loaded;
}

void* Texture::CreateDataBuffer() {
  int bytes_per_face = 0;

  switch (format_) {
  case TEXTURE_FORMAT_R8G8B8A8_UINT:
  case TEXTURE_FORMAT_R8G8B8A8_UNORM:
    bytes_per_face = 4 * width_ * height_;
    break;
  case TEXTURE_FORMAT_R32G32B32A32_FLOAT:
    bytes_per_face = 16 * width_ * height_;
    break;
  default:
    bytes_per_face = 0;
    break;
  }

  switch (type_) {
  case TEXTURE_TYPE_2D:
    data_ = malloc(bytes_per_face);
    break;
  case TEXTURE_TYPE_CUBE:
    data_ = malloc(bytes_per_face * 6);
    break;
  default:
    data_ = nullptr;
    break;
  }

  return data_;
}

void Texture::DestroyDataBuffer() {
  free(data_);
}
} // namespace scene
} // namespace magnet