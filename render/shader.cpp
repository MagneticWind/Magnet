#include <iostream>
#include "shader.h"

namespace magnet {
namespace render {
namespace {
static const char SHADER_PATH[256] = "C:\\Projects\\GitHub\\LightBaker\\data\\shader\\";
}

void VertexShader::Create(const void* source, int size,
  ID3D11Device* device) {
  device->CreateVertexShader(source, size, nullptr, &vertex_shader_);
}

void VertexShader::Set(ID3D11DeviceContext* device_context) {
  device_context->VSSetShader(vertex_shader_, 0, 0);
}

void VertexShader::Clear(ID3D11DeviceContext* device_context) {
  device_context->VSSetShader(0, 0, 0);
}

void PixelShader::Create(const void* source, int size,
  ID3D11Device* device) {
  device->CreatePixelShader(source, size, nullptr, &pixel_shader_);
}

void PixelShader::Set(ID3D11DeviceContext* device_context) {
  device_context->PSSetShader(pixel_shader_, 0, 0);
}

void PixelShader::Clear(ID3D11DeviceContext* device_context) {
  device_context->PSSetShader(0, 0, 0);
}

void ComputeShader::Create(const void* source, int size,
  ID3D11Device* device) {
  device->CreateComputeShader(source, size, nullptr, &compute_shader_);
}

void ComputeShader::Set(ID3D11DeviceContext* device_context) {
  device_context->CSSetShader(compute_shader_, 0, 0);
}

void ComputeShader::Clear(ID3D11DeviceContext* device_context) {
  device_context->CSSetShader(0, 0, 0);
}

ShaderProgram::ShaderProgram(const std::string& name) {
  name_ = name;
  for (int i = 0; i < MAX_SHADER_NUM; ++i) {
    shaders_[i] = nullptr;
    sources_[i] = 0;
    sizes_[i] = 0;
  }
}

ShaderProgram::~ShaderProgram() {
  for (int i = 0; i < MAX_SHADER_NUM; ++i) {
    if (sources_[i]) {
      free(sources_[i]);
      sources_[i] = nullptr;
    }

    if (shaders_[i]) {
      delete shaders_[i];
      shaders_[i] = nullptr;
    }
  }
}

void ShaderProgram::LoadShader(ShaderType type) {
  if (type == VERTEX_SHADER) {
    // load vertex shader
    const std::string path = SHADER_PATH + name_ + ".v";

    FILE* file = fopen(path.c_str(), "r+b");
    if (file == 0) {
      std::cout << "can't find shader; " << path << std::endl;
      return;
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    void* file_data = CreateBuffer(file_size, VERTEX_SHADER);
    int iResult = fread(file_data, 1, file_size, file);
    if (iResult != file_size) {
      std::cout << "Error reading shader file:" << path << std::endl;
      return;
    }

    fclose(file);
  } else if (type == PIXEL_SHADER) {
    const std::string path = SHADER_PATH + name_ + ".p";

    FILE* file = fopen(path.c_str(), "r+b");
    if (file == 0) {
      std::cout << "can't find shader; " << path << std::endl;
      return;
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    void* file_data = CreateBuffer(file_size, PIXEL_SHADER);
    int iResult = fread(file_data, 1, file_size, file);
    if (iResult != file_size) {
      std::cout << "Error reading shader file:" << path << std::endl;
      return;
    }

    fclose(file);
  } else if (type == COMPUTE_SHADER) {
    const std::string path = SHADER_PATH + name_ + ".c";

    FILE* file = fopen(path.c_str(), "r+b");
    if (file == 0) {
      std::cout << "can't find shader; " << path << std::endl;
      return;
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    void* file_data = CreateBuffer(file_size, COMPUTE_SHADER);
    int iResult = fread(file_data, 1, file_size, file);
    if (iResult != file_size) {
      std::cout << "Error reading shader file:" << path << std::endl;
      return;
    }

    fclose(file);
  }
}

void ShaderProgram::CreateShaders(ID3D11Device* device) {
  for (int i = 0; i < MAX_SHADER_NUM; ++i) {
    if (sizes_[i] > 0) {
      shaders_[i]->Create(sources_[i], sizes_[i], device);
    }
  }
}

void ShaderProgram::SetShaders(int iNumTextureLabels, int textureLabels[],
  ID3D11DeviceContext* device_context) {
  for (int i = 0; i < MAX_SHADER_NUM; ++i) {
    if (sizes_[i] > 0) {
      shaders_[i]->Set(device_context);
    }
  }
}

void ShaderProgram::ClearShaders(ID3D11DeviceContext* device_context) {
  for (int i = 0; i < MAX_SHADER_NUM; ++i) {
    if (sizes_[i] > 0) {
      shaders_[i]->Clear(device_context);
    }
  }
}

void* ShaderProgram::CreateBuffer(int size, ShaderType type) {
  sources_[type] = malloc(size);
  sizes_[type] = size;
  return sources_[type];
}

const std::string& ShaderProgram::GetName() const {
  return name_;
}

int ShaderProgram::GetFileSize(ShaderType type) const {
  return sizes_[type];
}

const void* ShaderProgram::GetFileData(ShaderType type) const {
  return sources_[type];
}

IShader* ShaderProgram::GetShader(ShaderType type) {
  return shaders_[type];
}
}  // namespace render
}  // namespace magnet