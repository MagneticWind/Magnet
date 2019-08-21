#ifndef MAGNET_RENDER_RENDER_MANAGER_H_
#define MAGNET_RENDER_RENDER_MANAGER_H_

#include <memory>
#include <mutex>
#include <vector>
#include <d3d11.h>
#include "render_pass.h"
#include "math\vector4.h"
#include "math\vector3.h"
#include "math\matrix4.h"

namespace magnet {
namespace render {
class ShaderProgram;
class Mesh;
class Surface;
class RenderManager {
 private:
  RenderManager();
  ~RenderManager();
  RenderManager(const RenderManager&) = delete;
  RenderManager& operator=(const RenderManager&) = delete;

  static RenderManager* instance_;

public:
  static RenderManager* GetInstance();
  static void Initialize(int width, int height, void* window_handle);
  static bool Exist();
  static void Terminate();

public:
  ID3D11Device* GetDevice();
  ID3D11DeviceContext* GetDeviceContext();
  ID3D11DeviceContext* GetDeferredDeviceContext();
  ID3D11RenderTargetView* GetFrameBufferRTV();
  ID3D11RenderTargetView* GetFrameBufferRTVHDR();
  ID3D11ShaderResourceView* GetFrameBufferSRVHDR();
  ID3D11ShaderResourceView* GetDepthSRV();
  ID3D11DepthStencilView* GetFrameBufferDSV();
  ID3D11RasterizerState* GetRasterizerState();
  D3D11_VIEWPORT GetViewPort();
  ID3D11DepthStencilState* GetDepthStencilState();
  ID3D11SamplerState* GetLinearSamplerState();
  ID3D11SamplerState* GetAnisotropicSamplerState();

  // executed by render thread
  void Render();
  void BeginRendering();
  void StopRendering();

  int GetUpdateFrameCount();
  void IncreaseUpdateFrameCount();
  int GetRenderFrameCount();

  // executed by game threads
  void Update(Surface* surface);
  void SwapDoubleBuffers();

  RenderPass* GetPass(PassType type);
  void SetFrameBufferDimension(int width, int height);
  void GetFrameBufferDimension(int* width, int* height) const;

  void SetMajorLightData(const math::Vector3f& direction, const math::Vector3f& color);

  void SetCameraData(const math::Matrix4f& view, const math::Matrix4f& projection);

  void GetSHCubemap(math::Vector4f shCoeffs[], int iNumCoeffs);

private:
  void InitializeDXSystem();
  void CopyShadowParameters();

private:
  void* window_handle_;

  D3D_DRIVER_TYPE driver_type_;
  D3D_FEATURE_LEVEL feature_level_;
  IDXGISwapChain* swap_chain_;

  // frame buffer
  ID3D11Texture2D* frame_buffer_;
  ID3D11ShaderResourceView* frame_buffer_srv_;
  ID3D11RenderTargetView* frame_buffer_rtv_;

  // depth
  ID3D11Texture2D* frame_buffer_depth_stencil_;
  ID3D11DepthStencilView* frame_buffer_dsv_;
  ID3D11ShaderResourceView* frame_buffer_depth_srv_;

  // hdr frame buffer
  ID3D11Texture2D* frame_buffer_hdr_;
  ID3D11RenderTargetView* frame_buffer_rtv_hdr_;
  ID3D11ShaderResourceView* frame_buffer_srv_hdr_;

  // depth stencil state
  ID3D11DepthStencilState* depth_stencil_enabled_;
  ID3D11DepthStencilState* depth_stencil_disabled_;

  // rasterization state
  ID3D11RasterizerState* rasterizer_state_;
  ID3D11BlendState* blend_color_write_enabled_;
  ID3D11BlendState* blend_color_write_diabled_;

  D3D11_VIEWPORT view_port_;

  ID3D11Device* device_;
  ID3D11DeviceContext* device_context_immediate_;
  ID3D11DeviceContext* device_context_deferred_;

  // quad mesh
  Mesh* quad_mesh_;
  ShaderProgram* tonemapping_shader_;
  ShaderProgram* shadow_shader_;

  bool postprocess_resources_created_;
  bool shadow_resources_created_;

  int width_;
  int height_;

  math::Matrix4f view_[2];
  math::Matrix4f projection_[2];
  int render_index_;
  int update_index_;

  std::vector<RenderPass*> render_passes_;

  // the frame number that game threads are updating
  int update_frame_count_;
  int render_frame_count_;
  std::mutex update_frame_count_mutex_;

  bool render_;
  bool stop_render_;
  std::mutex render_mutex_;

 public:
  std::mutex render_frame_count_mutex_;
};
}  // namespace magnet
}  // namespace render
#endif  // MAGNET_RENDER_RENDER_MANAGER_H_