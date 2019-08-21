#ifndef MAGNET_RENDER_GPU_RESOURCE_H_
#define MAGNET_RENDER_GPU_RESOURCE_H_

#include <string.h>
#include <d3d11.h>

#define MAX_NUM_ELEMENTS 6

namespace magnet {
namespace render {
struct MeshResource {
  MeshResource() : elements_count(0), primitives_count(0), stride(0),
    vertex_buffer(nullptr), index_buffer(nullptr)
  {}

  // semantic:
  // POSITION
  // NORMAL
  // TEXCOORD
  // COLOR
  // TANGENT
  // BINORMAL
  void AddElement(const std::string& semantic, int floats_count) {
    D3D11_INPUT_ELEMENT_DESC& desc = elements_desc[elements_count];
    if (floats_count == 3)
      desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    else if (floats_count == 2)
      desc.Format = DXGI_FORMAT_R32G32_FLOAT;
    else if (floats_count == 4)
      desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    desc.SemanticName = semantic.c_str();
    desc.InputSlot = 0;
    desc.AlignedByteOffset = stride;
    desc.SemanticIndex = 0;
    desc.InstanceDataStepRate = 0;
    desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    stride += floats_count * sizeof(float);
    elements_count++;
  }

  int elements_count;
  int primitives_count;
  int stride;

  char input_layout_str[64];

  ID3D11Buffer* vertex_buffer;
  ID3D11Buffer* index_buffer;

  D3D11_INPUT_ELEMENT_DESC elements_desc[MAX_NUM_ELEMENTS];
};

struct TextureResource {
  TextureResource() : label(""), texture(nullptr), sampler(nullptr) {
  }

  std::string label;
  ID3D11Texture2D* texture;
  ID3D11ShaderResourceView* srv;
  ID3D11SamplerState* sampler;
};
}  // namespace magnet
}  // namespace render
#endif  // MAGNET_RENDER_GPU_RESOURCE_H_