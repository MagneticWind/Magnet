#ifndef MAGNET_RENDER_RESOURCE_MANAGER_H_
#define MAGNET_RENDER_RESOURCE_MANAGER_H_

#include <map>
#include <string>
#include <d3d11.h>
#include "gpu_resource.h"
#include "texture.h"

namespace magnet {
namespace render {
class Mesh;
class Texture;
class ResourceManager {
 private:
   ResourceManager();
  ~ResourceManager();
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  static ResourceManager* instance_;

 public:
  static ResourceManager* GetInstance();
  static void Initialize();
  static bool Exist();
  static void Terminate();

 public:
  void CreateMeshResource(const Mesh* mesh, ID3D11Device* device);
  void CreateTextureResource(const Texture* pTexture, ID3D11Device* device);
  //void CreateCubeTextureResource(const Scene::Texture* pTexture, HALgfx::IDevice* pDevice);
  void CreateSamplerState(const SamplerMode mode, ID3D11Device* device);

  MeshResource& GetMeshResource(const std::string& name);
  TextureResource& GetTextureResource(const std::string& name);
  ID3D11SamplerState* GetSamplerState(int samplerMode, ID3D11Device* device);

private:
  std::map<std::string, MeshResource> mesh_map_;
  std::map<std::string, TextureResource> texture_map_;
  std::map<int, ID3D11SamplerState*> sampler_map_;
};

}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_RESOURCE_MANAGER_H_