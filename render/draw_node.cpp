#include <stdlib.h>

#include "draw_node.h"


namespace magnet {
namespace render {

DrawNode::DrawNode() : samplers_count(0), srvs_count(0), 
  vertex_buffer(nullptr), index_buffer(nullptr), vs_cbuffers_count(0),
  ps_cbuffers_count(0), set_view_port(false) {
  for (int i = 0; i < MAX_NUMBER_SRVS; ++i) {
    srvs[i] = nullptr;
  }
  for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i) {
    vs_cbuffer_data[i] = nullptr;
    ps_cbuffer_data[i] = nullptr;
  }
}

DrawNode::~DrawNode() {
  for (int i = 0; i < MAX_NUMBER_SRVS; ++i) {
    srvs[i] = nullptr;
  }

  for (int i = 0; i < MAX_NUMBER_SAMPLERS; ++i) {
    samplers[i] = nullptr;
  }

  for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i) {
    vs_cbuffer_data[i] = nullptr;
    ps_cbuffer_data[i] = nullptr;
  }

  vertex_buffer = nullptr;
  index_buffer = nullptr;
}

void DrawNode::AddSRV(ID3D11ShaderResourceView* srv) {
  srvs[srvs_count++] = srv;
}

void DrawNode::AddSampler(ID3D11SamplerState* sampler) {
  samplers[samplers_count++] = sampler;
}

void DrawNode::RemoveSRV(ID3D11ShaderResourceView* srv) {
  for (int i = 0; i < srvs_count; ++i) {
    if (srvs[i] == srv) {
      srvs[i] = nullptr;
      // move the rest of elements forward
      for (int j = i; j < srvs_count - 1; ++j) {
        srvs[j] = srvs[j + 1];
      }
      break;
    }
  }

  srvs_count--;
}

void DrawNode::CreateCBufferData(int size, ShaderType type)
{
  void* pBuffer = 0;

  if (type == VERTEX_SHADER) {
    pBuffer = malloc(size);
    vs_cbuffer_data[vs_cbuffers_count++] = pBuffer;
  } else if (type == PIXEL_SHADER) {
    pBuffer = malloc(size);
    ps_cbuffer_data[ps_cbuffers_count++] = pBuffer;
  }
}

void* DrawNode::GetCBufferData(int index, ShaderType type) {
  if (type == VERTEX_SHADER) {
    return vs_cbuffer_data[index];
  } else if (type == PIXEL_SHADER) {
    return ps_cbuffer_data[index];
  }
}

void DrawNode::DestroyCBufferData(void* pBuffer)
{
  free(pBuffer);
}

} // namespace Renderer
} // namespace Magnet