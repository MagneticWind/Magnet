#ifndef MAGNET_RENDER_DRAW_NODE_H_
#define MAGNET_RENDER_DRAW_NODE_H_

#include <d3d11.h>
#include <string>
#include "math\matrix4.h"
#include "math\vector3.h"
#include "shader.h"

namespace magnet {
namespace render {
#define MAX_NUMBER_BUFFERS 8
#define MAX_NUMBER_SAMPLERS 8
#define MAX_NUMBER_SRVS 8

struct DrawNode {
  DrawNode();
  ~DrawNode();

  void AddSRV(ID3D11ShaderResourceView* srv);
  void AddSampler(ID3D11SamplerState* sampler_state);
  void RemoveSRV(ID3D11ShaderResourceView* srv);

  void CreateCBufferData(int size, ShaderType type);
  void* GetCBufferData(int index, ShaderType type);
  void DestroyCBufferData(void* data);

  // geometry
  ID3D11Buffer* vertex_buffer;
  ID3D11Buffer* index_buffer;

  // c buffers
  void* vs_cbuffer_data[MAX_NUMBER_BUFFERS];
  void* ps_cbuffer_data[MAX_NUMBER_BUFFERS];

  // textures
  ID3D11ShaderResourceView* srvs[MAX_NUMBER_SRVS];

  // 
  ID3D11SamplerState* samplers[MAX_NUMBER_SAMPLERS];

  std::string name;

  int samplers_count;
  int srvs_count;
  int primitives_count;
  int vs_cbuffers_count;
  int ps_cbuffers_count;

  bool set_view_port;
  D3D11_VIEWPORT view_port;

  math::Matrix4f world_;
};
}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_DRAW_NODE_H_
