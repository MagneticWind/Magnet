#ifndef MAGNET_RENDER_RENDER_PASS_OPAQUE_H_
#define MAGNET_RENDER_RENDER_PASS_OPAQUE_H_

#include <map>
#include <mutex>
#include <d3d11.h>
#include <string>
#include "math/matrix4.h"
#include "math/vector4.h"
#include "cbuffer_desc.h"
#include "render_pass.h"
#include "shader.h"

#define MAX_CASCADE_COUNT 4
namespace magnet {
namespace render {
typedef void(*CallBackCopyShadowParameters) (math::Matrix4f* shadow_view,
  math::Matrix4f** projection, ID3D11ShaderResourceView* shadowmap_srv);

class RenderPassOpaque : public RenderPass {
 public:
  RenderPassOpaque();
  ~RenderPassOpaque();

  void Render(ID3D11DeviceContext* device_context,
    const D3D11_VIEWPORT& view_port, ID3D11RenderTargetView* rtv,
    ID3D11RenderTargetView* rtv_hdr,
    ID3D11DepthStencilView* dsv, ID3D11RasterizerState* raster_state,
    ID3D11DepthStencilState* depth_stencil_state_enable,
    ID3D11DepthStencilState* depth_stencil_state_disable) override;
 
  PassType GetType() override;

  void SetShadowParameters(math::Matrix4f* mShadowView, math::Matrix4f* mProjection,
    ID3D11ShaderResourceView** pShadowmapSRV, math::Vector4f* pRange);
  void Update(ID3D11Device* device, Surface* surface) override;

  void SwapDoubleBuffers() override;

 private:
  // used by multiple threads(including render thread):
  // create new shader node, create gpu resource, update it's drawnodes 
  std::map<std::string, ShaderNode*> shader_nodes_;
  std::mutex shader_nodes_mutex_;

  CBufferLights lights_buffer_[2];
  math::Matrix4f view_[2];
  math::Matrix4f projection_[2];

  int render_index_;
  int update_index_;

  math::Matrix4f shadow_view_;
  math::Matrix4f shadow_projection_[MAX_CASCADE_COUNT];
  D3D11_VIEWPORT shadow_view_ports_[MAX_CASCADE_COUNT];
  math::Vector4f shadow_range_;
  ID3D11ShaderResourceView* shadow_srv_;
};
}  // namespace magnet
}  // namespace render
#endif  // MAGNET_RENDER_RENDER_PASS_OPAQUE_H_
