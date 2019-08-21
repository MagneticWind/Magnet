#ifndef MAGNET_RENDER_SHADER_H_
#define MAGNET_RENDER_SHADER_H_

#include <string>
#include <d3d11.h>

namespace magnet {
namespace render {
enum ShaderType {
  VERTEX_SHADER = 0,
  GEOMETRY_SHADER,
  HULL_SHADER,
  DOMAIN_SHADER,
  PIXEL_SHADER,
  COMPUTE_SHADER,
  MAX_SHADER_NUM
};

class IShader {
 public:
  virtual ~IShader() {}
  virtual void Create(const void* source, int size, ID3D11Device* device) = 0;
  virtual void Set(ID3D11DeviceContext* device_context) = 0;
  virtual void Clear(ID3D11DeviceContext* device_context) = 0;
};

class VertexShader : public IShader {
 public:
  VertexShader() : vertex_shader_(nullptr) {}
  ~VertexShader() {}
  void Create(const void* source, int size, ID3D11Device* device) override;
  void Set(ID3D11DeviceContext* device_context) override;
  void Clear(ID3D11DeviceContext* device_context) override;
 private:
  ID3D11VertexShader* vertex_shader_;
};

class PixelShader : public IShader {
 public:
  PixelShader() : pixel_shader_(nullptr) {}
  ~PixelShader() {}
  void Create(const void* source, int iSize, ID3D11Device* device) override;
  void Set(ID3D11DeviceContext* device_context) override;
  void Clear(ID3D11DeviceContext* device_context) override;
  ID3D11PixelShader* pixel_shader_;
};

class ComputeShader : public IShader {
public:
  ComputeShader() : compute_shader_(nullptr) {}
  ~ComputeShader() {}
  void Create(const void* source, int size, ID3D11Device* device) override;
  void Set(ID3D11DeviceContext* device_context) override;
  void Clear(ID3D11DeviceContext* device_context) override;
  ID3D11ComputeShader* compute_shader_;
};

class ShaderProgram {
 public:
  explicit ShaderProgram(const std::string& name);
  ~ShaderProgram();

  const std::string& GetName() const;
  int GetFileSize(ShaderType type) const;
  const void* GetFileData(ShaderType type) const;
  IShader* GetShader(ShaderType type);

  void LoadShader(ShaderType type);
  void CreateShaders(ID3D11Device* device);
  void SetShaders(int textures_count, int textures[],
    ID3D11DeviceContext* device_context);
  void ClearShaders(ID3D11DeviceContext* device_context);

 private:
   void* CreateBuffer(int size, ShaderType type);

 private:
  std::string name_;
  IShader* shaders_[MAX_SHADER_NUM];
  void* sources_[MAX_SHADER_NUM];
  int sizes_[MAX_SHADER_NUM];
};
}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_SHADER_PROGRAM_H_
