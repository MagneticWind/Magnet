#include "resource_manager.h"
#include "mesh.h"

namespace magnet {
namespace render {

ResourceManager* ResourceManager::instance_ = nullptr;

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ResourceManager* ResourceManager::GetInstance() {
  return instance_;
}

void ResourceManager::Initialize() {
  instance_ = new ResourceManager();
}

bool ResourceManager::Exist() {
  return instance_ = nullptr;
}

void ResourceManager::Terminate() {
  delete instance_;
}

void ResourceManager::CreateMeshResource(const Mesh* mesh,
  ID3D11Device* device) {
  const std::string& name = mesh->GetName();
  auto it = mesh_map_.find(name);
  if (it == mesh_map_.end()) {
    MeshResource mesh_resource;

    // input elements
    int size = 0;
    VertexDecl decls[MAX_DESC_COUNT];
    mesh->GetVetexDecls(&size, decls);
    for (int i = 0; i < size; ++i) {
      switch (decls[i]) {
      case POSITION:
        mesh_resource.AddElement("POSITION", 3);
        break;
      case NORMAL:
        mesh_resource.AddElement("NORMAL", 3);
        break;
      case UV:
        mesh_resource.AddElement("UV", 2);
        break;
      }
    }

    // vertex buffer
    D3D11_BUFFER_DESC desc;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = mesh_resource.stride * mesh->GetVertsCount();
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.Usage = D3D11_USAGE_DEFAULT;
    //desc.miscFlags
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = mesh->GetVertexDataPtr();
    device->CreateBuffer(&desc, &data, &mesh_resource.vertex_buffer);

    // index buffer
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.ByteWidth = sizeof(unsigned int) * 3 * mesh->GetFacesCount();
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.Usage = D3D11_USAGE_DEFAULT;
    data.pSysMem = mesh->GetIndexDataPtr();
    device->CreateBuffer(&desc, &data, &mesh_resource.index_buffer);

    mesh_resource.primitives_count = mesh->GetFacesCount();

    mesh_map_.insert(std::pair<const std::string&, MeshResource>(name, mesh_resource));
  }
}

void ResourceManager::CreateTextureResource(const Texture* texture,
  ID3D11Device* device) {
  TextureResource texture_resource;

  const std::string& name = texture->GetName();

  auto it = texture_map_.find(name);
  if (it == texture_map_.end()) {
    if (texture->GetType() == TEXTURE_TYPE_2D) {
      D3D11_TEXTURE2D_DESC desc;
      desc.Width = texture->GetWidth();
      desc.Height = texture->GetHeight();

      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

      D3D11_SUBRESOURCE_DATA data;
      data.pSysMem = texture->GetDataBufferPtr();
      switch (texture->GetFormat()) {
      case TEXTURE_FORMAT_R8G8B8A8_UINT:
        data.SysMemPitch = 4 * texture->GetWidth();
        desc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
        break;
      case TEXTURE_FORMAT_R8G8B8A8_UNORM:
        data.SysMemPitch = 4 * texture->GetWidth();
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
      }

      device->CreateTexture2D(&desc, &data, &texture_resource.texture);

      D3D11_SHADER_RESOURCE_VIEW_DESC desc_srv;
      desc_srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      desc_srv.Texture2D.MostDetailedMip = 0;
      desc_srv.Texture2D.MipLevels = desc.MipLevels;
      desc_srv.Format = desc.Format;
      device->CreateShaderResourceView(texture_resource.texture, &desc_srv,
        &texture_resource.srv);
    }
    else if (texture->GetType() == TEXTURE_TYPE_CUBE) {
/*      D3D11_TEXTURE2D_DESC texDesc;
      texDesc.width = pTexture->GetWidth();
      texDesc.height = pTexture->GetHeight();
      texDesc.arraySize = 6;
      texDesc.miscFlags = HALgfx::MISC_TEXTURECUBE;
      texDesc.bindFlags = HALgfx::BIND_SHADER_RESOURCE;
      texDesc.mipLevels = 1; // this has to be set for cubemap, it can't be 9

      HALgfx::SubResourceData data[6];

      void* pDataPtr = const_cast<void*>(pTexture->GetDataBufferPtr());
      for (int i = 0; i < 6; ++i)
      {
        switch (pTexture->GetFormat())
        {
        case Scene::TEXTURE_FORMAT_R8G8B8A8_UINT:
          data[i].uMemPitch = 4 * pTexture->GetWidth();
          data[i].uMemSlicePitch = 0;
          texDesc.format = HALgfx::FORMAT_R8G8B8A8_UINT;
          data[i].pMem = static_cast<unsigned char*>(pDataPtr) + i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
          break;
        case Scene::TEXTURE_FORMAT_R8G8B8A8_UNORM:
          data[i].uMemPitch = 4 * pTexture->GetWidth();
          data[i].uMemSlicePitch = 0;
          texDesc.format = HALgfx::FORMAT_R8G8B8A8_UNORM;
          data[i].pMem = static_cast<unsigned char*>(pDataPtr) + i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
          break;
        case Scene::TEXTURE_FORMAT_R32G32B32A32_FLOAT:
          data[i].uMemPitch = 16 * pTexture->GetWidth();
          data[i].uMemSlicePitch = 0;
          texDesc.format = HALgfx::FORMAT_R32G32B32A32_FLOAT;
          data[i].pMem = static_cast<float*>(pDataPtr) + i * 4 * pTexture->GetWidth() * pTexture->GetHeight();
          break;
        }
      }

      textureResource.m_pTexture = pDevice->CreateTexture2d(texDesc, data);

      HALgfx::ShaderResourceViewDesc desc;
      desc.viewDimension = HALgfx::SRV_DIMENSION_TEXTURECUBE;
      desc.texSRV.mostDetailedMip = 0;
      desc.texSRV.mipLevels = texDesc.mipLevels;
      desc.format = texDesc.format;
      textureResource.m_pShaderResourceView = pDevice->CreateShaderResourceView(textureResource.m_pTexture, desc);
      */
    }

    // set label name
    switch (texture->GetLabel())
    {
    case TEXTURE_LABEL_COLOR_0:
      texture_resource.label = "txColor0";
      break;
    case TEXTURE_LABEL_COLOR_1:
      texture_resource.label = "txColor1";
      break;
    case TEXTURE_LABEL_NORMAL:
      texture_resource.label = "txNormal";
      break;
    case TEXTURE_LABEL_SPECULAR:
      texture_resource.label = "txSpecular";
      break;
    case TEXTURE_LABEL_EMISSIVE:
      texture_resource.label = "txEmissive";
      break;
    case TEXTURE_LABEL_SKY:
      texture_resource.label = "txSky";
      break;
    case TEXTURE_LABEL_SHADOW:
      texture_resource.label = "txShadow";
      break;
    }

    CreateSamplerState(texture->GetSamplerMode(), device);
    texture_resource.sampler = GetSamplerState(texture->GetSamplerMode(), device);

    texture_map_[name] = texture_resource;
  }
}

//void CreateCubeTextureResource(const Scene::Texture* pTexture, HALgfx::IDevice* pDevice);

void ResourceManager::CreateSamplerState(const SamplerMode mode, ID3D11Device* device) {
  auto it = sampler_map_.find(mode);
  if (it == sampler_map_.end()) {
    switch (mode) {
    case SAMPLER_MIP_LINEAR_WRAP: 
    {
      // linear sampler state
      D3D11_SAMPLER_DESC desc;
      desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
      desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
      desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
      desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
      desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
      desc.MaxAnisotropy = 1;
      desc.MaxLOD = 0;
      desc.MinLOD = 0;
      desc.MipLODBias = 0.f;
      ID3D11SamplerState* sampler_state;
      device->CreateSamplerState(&desc, &sampler_state);

      sampler_map_[mode] = sampler_state;
      break;
    }
    case SAMPLER_NOMIP_LINEAR_WRAP:
    {
      //// linear sampler state
      //D3D11_SAMPLER_DESC desc;
      //desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
      //desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
      //desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
      //desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
      //desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR;
      //desc.MaxAnisotropy = 1;
      //desc.MaxLOD = 0;
      //desc.MinLOD = 0;
      //desc.MipLODBias = 0.f;
      //ID3D11SamplerState* sampler_state;
      //device->CreateSamplerState(&desc, &sampler_state);

      //sampler_map_[mode] = sampler_state;
      //break;
    }
    default:
      break;
    }
  }
}

MeshResource& ResourceManager::GetMeshResource(const std::string& name) {
  return mesh_map_[name];
}

TextureResource& ResourceManager::GetTextureResource(const std::string& name) {
  return texture_map_[name];
}

ID3D11SamplerState* ResourceManager::GetSamplerState(int sampler, ID3D11Device* device) {
  return sampler_map_[sampler];
}
}  // namespace render
}  // namespace magnet