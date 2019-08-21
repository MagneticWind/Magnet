
#include "render_pass.h"
#include "render_manager.h"
#include "render_pass_opaque.h"
//#include "RenderPassPostprocess.h"
//#include "RenderPassDepth.h"
//#include "RenderPassSky.h"
//#include "RenderPassShadow.h"

#include "surface.h"
#include "mesh.h"
#include "texture.h"
#include "material.h"

namespace magnet {
namespace render {
RenderManager* RenderManager::instance_ = nullptr;

RenderManager::RenderManager() {
  postprocess_resources_created_ = false;
  shadow_resources_created_ = false;
  render_frame_count_ = 0;
  update_frame_count_ = 0;
  render_ = false;
  stop_render_ = false;
}

RenderManager::~RenderManager() {
}

RenderManager* RenderManager::GetInstance() {
  return instance_;
}

void RenderManager::Initialize(int width, int height, void* window_handle) {

  instance_ = new RenderManager();
  instance_->SetFrameBufferDimension(width, height);
  instance_->window_handle_ = window_handle;

  instance_->InitializeDXSystem();

  instance_->render_passes_.emplace_back(new RenderPassOpaque());
}

void RenderManager::InitializeDXSystem() {
  HRESULT hr = S_OK;

  UINT createDeviceFlags = 0;

#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width_;
  sd.BufferDesc.Height = height_;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
  sd.OutputWindow = static_cast<HWND>(window_handle_);
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  driver_type_ = D3D_DRIVER_TYPE_HARDWARE;
  hr = D3D11CreateDeviceAndSwapChain(NULL, driver_type_, NULL,
    createDeviceFlags, featureLevels, numFeatureLevels,
    D3D11_SDK_VERSION, &sd, &swap_chain_, &device_,
    &feature_level_, &device_context_immediate_);

  if (FAILED(hr)) {
    printf("Failed to create DirectX 11 system.");
    return;
  }

  // final render target
  hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& frame_buffer_);
  D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
  rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Texture2D.MipSlice = 0;
  device_->CreateRenderTargetView(frame_buffer_, &rtv_desc, &frame_buffer_rtv_);

  // view port
  view_port_.Width = width_;
  view_port_.Height = height_;
  view_port_.TopLeftX = 0;
  view_port_.TopLeftY = 0;
  view_port_.MinDepth = 0.0f;
  view_port_.MaxDepth = 1.f;

  // HDR render target texture
  D3D11_TEXTURE2D_DESC texture2dDesc;
  texture2dDesc.Width = width_;
  texture2dDesc.Height = height_;
  texture2dDesc.ArraySize = 1;
  texture2dDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
  texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
  texture2dDesc.MipLevels = 1;
  texture2dDesc.CPUAccessFlags = 0;
  texture2dDesc.MiscFlags = 0;
  texture2dDesc.SampleDesc.Count = 1;
  texture2dDesc.SampleDesc.Quality = 0;
  device_->CreateTexture2D(&texture2dDesc, 0, &frame_buffer_hdr_);

  // HDR SRV
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  srvDesc.Format = texture2dDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  srvDesc.Texture2D.MostDetailedMip = 0;

  device_->CreateShaderResourceView(frame_buffer_hdr_, &srvDesc, &frame_buffer_srv_hdr_);

  // HDR render target
  D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
  rtvDesc.Format = texture2dDesc.Format;
  rtvDesc.Texture2D.MipSlice = 0;
  rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  device_->CreateRenderTargetView(frame_buffer_hdr_, &rtvDesc, &frame_buffer_rtv_hdr_);

  // depth stencil target texture
  D3D11_TEXTURE2D_DESC texture2dDSTDesc;
  texture2dDSTDesc.Width = width_;
  texture2dDSTDesc.Height = height_;
  texture2dDSTDesc.ArraySize = 1;
  texture2dDSTDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
  texture2dDSTDesc.Usage = D3D11_USAGE_DEFAULT;
  texture2dDSTDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
  texture2dDSTDesc.MipLevels = 1;
  texture2dDSTDesc.CPUAccessFlags = 0;
  texture2dDSTDesc.MiscFlags = 0;
  texture2dDSTDesc.SampleDesc.Count = 1;
  texture2dDSTDesc.SampleDesc.Quality = 0;

  device_->CreateTexture2D(&texture2dDSTDesc, 0, &frame_buffer_depth_stencil_);

  // depth stencil target
  D3D11_DEPTH_STENCIL_VIEW_DESC dstDesc;
  dstDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  dstDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dstDesc.Texture2D.MipSlice = 0;
  dstDesc.Flags = 0; // not read only
  device_->CreateDepthStencilView(frame_buffer_depth_stencil_, &dstDesc, &frame_buffer_dsv_);

  // depth SRV
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDescDepth;
  srvDescDepth.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
  srvDescDepth.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDescDepth.Texture2D.MipLevels = 1;
  srvDescDepth.Texture2D.MostDetailedMip = 0;
  device_->CreateShaderResourceView(frame_buffer_depth_stencil_, &srvDescDepth, &frame_buffer_depth_srv_);

  // depth stencil state
  D3D11_DEPTH_STENCIL_DESC sdDesc;
  sdDesc.DepthEnable = true;
  sdDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  sdDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  sdDesc.StencilEnable = false;
  sdDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  sdDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  sdDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  sdDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  sdDesc.BackFace = sdDesc.FrontFace;
  device_->CreateDepthStencilState(&sdDesc, &depth_stencil_enabled_);

  sdDesc.DepthEnable = false;
  sdDesc.StencilEnable = false;
  sdDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

  device_->CreateDepthStencilState(&sdDesc, &depth_stencil_disabled_);

  // rasterizer state
  D3D11_RASTERIZER_DESC rDesc;
  rDesc.CullMode = D3D11_CULL_BACK;
  rDesc.FrontCounterClockwise = false;
  rDesc.DepthBias = 0;
  rDesc.DepthClipEnable = true;
  rDesc.SlopeScaledDepthBias = 0.f;
  rDesc.ScissorEnable = false;
  rDesc.DepthBiasClamp = 0.f;
  rDesc.FillMode = D3D11_FILL_SOLID;
  rDesc.AntialiasedLineEnable = true;
  rDesc.MultisampleEnable = true;
  device_->CreateRasterizerState(&rDesc, &rasterizer_state_);
}

bool RenderManager::Exist() {
  return instance_ != nullptr;
}

void RenderManager::Terminate() {
  delete instance_;
  instance_ = nullptr;
}

ID3D11Device* RenderManager::GetDevice() {
  return device_;
}

ID3D11DeviceContext* RenderManager::GetDeviceContext() {
  return device_context_immediate_;
}

ID3D11DeviceContext* RenderManager::GetDeferredDeviceContext() {
  return device_context_deferred_;
}

ID3D11RenderTargetView* RenderManager::GetFrameBufferRTV() {
  return frame_buffer_rtv_;
}

ID3D11RenderTargetView* RenderManager::GetFrameBufferRTVHDR() {
  return frame_buffer_rtv_hdr_;
}

ID3D11ShaderResourceView* RenderManager::GetFrameBufferSRVHDR() {
  return frame_buffer_srv_hdr_;
}

ID3D11ShaderResourceView* RenderManager::GetDepthSRV() {
  return frame_buffer_depth_srv_;
}

ID3D11DepthStencilView* RenderManager::GetFrameBufferDSV() {
  return frame_buffer_dsv_;
}

ID3D11RasterizerState* RenderManager::GetRasterizerState() {
  return rasterizer_state_;
}

D3D11_VIEWPORT RenderManager::GetViewPort() {
  return view_port_;
}

ID3D11DepthStencilState* RenderManager::GetDepthStencilState() {
  return depth_stencil_enabled_;
}

RenderPass* RenderManager::GetPass(PassType type) {
  for (RenderPass* render_pass : render_passes_) {
    if (render_pass->GetType() == type)
      return render_pass;
  }
  return nullptr;
}

void RenderManager::SetFrameBufferDimension(int width, int height) {
  width_ = width;
  height_ = height;
}

void RenderManager::GetFrameBufferDimension(int* width, int* height) const {
  *width = width_;
  *height = height_;
}

void RenderManager::SetCameraData(const math::Matrix4f& view, const math::Matrix4f& projection) {
  view_[update_index_] = view;
  projection_[update_index_] = projection;
}

void RenderManager::Render() {
  while (true) {
    std::lock_guard<std::mutex> guard(render_mutex_);
    if (render_) {
      // render one frame, iterate render passes
      for (RenderPass* render_pass : render_passes_) {
        render_pass->Render(device_context_immediate_, view_port_, frame_buffer_rtv_,
          frame_buffer_rtv_hdr_, frame_buffer_dsv_, rasterizer_state_,
          depth_stencil_enabled_, depth_stencil_disabled_);
      }
      swap_chain_->Present(0, 0);

      // wait till next frame update finishes
      {
        std::lock_guard<std::mutex> guard(render_frame_count_mutex_);
        render_frame_count_++;
      }
      while (render_frame_count_ == update_frame_count_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }

    }
    else if (stop_render_) {
      return;
    }
    else {
      // sleep when begin rendering, waiting for the first frame update
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}

void RenderManager::StopRendering() {
  std::lock_guard<std::mutex> guard(render_mutex_);
  render_ = false;
  stop_render_ = true;
}

void RenderManager::BeginRendering() {
  std::lock_guard<std::mutex> guard(render_mutex_);
  render_ = true;
}

int RenderManager::GetUpdateFrameCount() {
  std::lock_guard<std::mutex> guard(update_frame_count_mutex_);
  return update_frame_count_;
}

int RenderManager::GetRenderFrameCount() {
  std::lock_guard<std::mutex> guard(render_frame_count_mutex_);
  return render_frame_count_;
}

// called from main thread
void RenderManager::IncreaseUpdateFrameCount() {
  std::lock_guard<std::mutex> guard(update_frame_count_mutex_);
  update_frame_count_++;
}

void RenderManager::Update(Surface* surface) {
  for (RenderPass* render_pass : render_passes_) {
    render_pass->Update(device_, surface);
  }
}

void RenderManager::SwapDoubleBuffers() {
  std::swap(render_index_, update_index_);

  for (RenderPass* render_pass : render_passes_) {
    render_pass->SwapDoubleBuffers();
  }
}

}  // namespace render
}  // namespace magnet