#include <vector>
#include "material.h"
#include "surface.h"
#include "render_pass_opaque.h"
#include "shader_node.h"
#include "cbuffer_desc.h"
#include "resource_manager.h"

namespace magnet {
namespace render {
namespace {
static const int kMaxDrawNodesCount = 30;
}  // namespace

RenderPassOpaque::RenderPassOpaque() {
  shadow_srv_ = nullptr;
}

RenderPassOpaque::~RenderPassOpaque() {
  for (auto it : shader_nodes_) {
    delete it.second;
  }
  shader_nodes_.clear();
}

void RenderPassOpaque::Render(ID3D11DeviceContext* device_context,
  const D3D11_VIEWPORT& view_port, ID3D11RenderTargetView* rtv,
  ID3D11RenderTargetView* rtv_hdr,
  ID3D11DepthStencilView* dsv, ID3D11RasterizerState* raster_state,
  ID3D11DepthStencilState* depth_stencil_state_enable,
  ID3D11DepthStencilState* depth_stencil_state_disable) {

  device_context->RSSetViewports(1, &view_port);
  device_context->RSSetState(raster_state);
  device_context->OMSetDepthStencilState(depth_stencil_state_enable, 0);
  device_context->OMSetRenderTargets(1, &rtv, dsv);

  float clear_color[4] = {0.f, 0.f, 0.f, 0.f};
  device_context->ClearRenderTargetView(rtv, clear_color);
  device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.f, 0);

  device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  ShaderNode::BeginEvent("RenderPassOpaque");
  for (auto it : shader_nodes_) {
    it.second->Draw(device_context);
  }
  ShaderNode::EndEvent();

  for (auto it : shader_nodes_) {
    it.second->ClearDrawNodes();
  }
}

PassType RenderPassOpaque::GetType() {
  return PASS_OPAQUE;
}

void RenderPassOpaque::Update(ID3D11Device* device, Surface* surface) {
  std::lock_guard<std::mutex> guard(shader_nodes_mutex_);

  std::shared_ptr<Material> material = surface->GetMaterial();
  std::string shader_name;
  ShaderNode* shader_node = nullptr;
  auto it = shader_nodes_.find(shader_name);
  if (it != shader_nodes_.end()) {
    shader_node = it->second;
  }
  else {
    shader_node = new ShaderNode(shader_name, kMaxDrawNodesCount);
    shader_node->LoadShader(VERTEX_SHADER, device);
    shader_node->LoadShader(PIXEL_SHADER, device);

    // transformation buffer
    D3D11_BUFFER_DESC desc;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = sizeof(CBufferTransform);
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;

    shader_node->CreateConstantBuffer(desc, device, VERTEX_SHADER);

    // material buffer
    desc.ByteWidth = sizeof(CBufferMaterialNormal);
    shader_node->CreateConstantBuffer(desc, device, PIXEL_SHADER);

    // input layout, it requires v shader byte code, and input elements from mesh
    const std::string& mesh_name = surface->GetMesh()->GetName();
    ResourceManager* resource_manager = ResourceManager::GetInstance();
    MeshResource& mesh_resource = resource_manager->GetMeshResource(mesh_name);

    shader_node->Create(mesh_resource, device);

    // initialize drawnodes
    for (int i = 0; i < kMaxDrawNodesCount; ++i) {
      DrawNode draw_node;
      draw_node.CreateCBufferData(sizeof(CBufferTransform), VERTEX_SHADER);
      draw_node.CreateCBufferData(sizeof(CBufferMaterialNormal), PIXEL_SHADER);
      draw_node.CreateCBufferData(sizeof(CBufferLights), PIXEL_SHADER);
      shader_node->AddDrawNode(draw_node);
    }

    shader_nodes_.insert(std::pair<std::string, ShaderNode*>(shader_name, shader_node));
  }

  DrawNode& draw_node = shader_node->GetCurrentDrawNode();
  std::string mesh_name = surface->GetMesh()->GetName();
  draw_node.name = mesh_name;

  draw_node.world_ = surface->GetWorld();

  ResourceManager* resource_manager = ResourceManager::GetInstance();
  MeshResource& meshResource = resource_manager->GetMeshResource(mesh_name);

  // vertex buffer and index buffer
  draw_node.vertex_buffer = meshResource.vertex_buffer;
  draw_node.index_buffer = meshResource.index_buffer;
  draw_node.primitives_count = meshResource.primitives_count;

  // const buffers data update
  CBufferMaterialNormal* material_buffer = static_cast<CBufferMaterialNormal*>(
    draw_node.GetCBufferData(0, render::PIXEL_SHADER));
  material->GetAmbient(&material_buffer->ambient);
  material->GetDiffuse(&material_buffer->diffuse);
  material->GetEmission(&material_buffer->emissive);
  material->GetSpecular(&material_buffer->specular);
  material_buffer->specular.w_ = material->GetExponent();

  // textures
  int textures_count = material->GetTexturesCount();
  for (int i = 0; i < textures_count; ++i) {
    const render::Texture* texture = material->GetTexture(i);
    std::string texName(texture->GetName());
    TextureResource& texture_resource = resource_manager->GetTextureResource(texName);
    draw_node.AddSRV(texture_resource.srv);
    draw_node.AddSampler(texture_resource.sampler);
  }

  shader_node->IncreaseDrawNodeIndex();
}

void RenderPassOpaque::SwapDoubleBuffers() {
  std::swap(render_index_, update_index_);
  for (auto shader_node : shader_nodes_) {
    
  }
 }

void RenderPassOpaque::SetShadowParameters(math::Matrix4f* shadow_view,
  math::Matrix4f* projection, ID3D11ShaderResourceView** shadow_srv,
  math::Vector4f* range) {
  shadow_view_ = *shadow_view;

  for (int i = 0; i < MAX_CASCADE_COUNT; ++i) {
    shadow_projection_[i] = projection[i];
  }
  shadow_range_ = *range;
  shadow_srv_ = *shadow_srv;
}
}  // namespace render
}  // namespace magnet