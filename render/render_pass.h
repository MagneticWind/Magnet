#ifndef MAGNET_RENDER_RENDER_PASS_H_
#define MAGNET_RENDER_RENDER_PASS_H_

#include <d3d11.h>
#include <list>

namespace magnet {
namespace render {
class IRenderObject;
class ShaderNode;
class Surface;

enum PassType {
  PASS_DEPTH,
  PASS_SKY,
  PASS_SHADOW,
  PASS_OPAQUE,
  PASS_POSTPROCESS,
  PASS_NUMBER,
  PASS_NULL
};

class RenderPass {
 public:
  RenderPass() {}
  virtual ~RenderPass() {}

  // executed by render thread
  virtual void Render(ID3D11DeviceContext* device_context,
    const D3D11_VIEWPORT& view_port, ID3D11RenderTargetView* rtv,
    ID3D11RenderTargetView* rtv_hdr,
    ID3D11DepthStencilView* dsv, ID3D11RasterizerState* raster_state,
    ID3D11DepthStencilState* depth_stencil_state_enable,
    ID3D11DepthStencilState* depth_stencil_state_disable) = 0;

  virtual PassType GetType() = 0;

  // executed by game threads
  virtual void Update(ID3D11Device* device, Surface* surface) = 0;

  virtual void SwapDoubleBuffers() = 0;
};
}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_RENDER_PASS_H_
