#include <stdlib.h>
#include <d3d11.h>
#include <d3d9.h>

#include "math\vector3.h"
#include "render_manager.h"

#include "material.h"
#include "texture.h"
#include "shader.h"
#include "shader_node.h"

namespace magnet {
namespace render {
ShaderNode::ShaderNode(const std::string& name, int draw_nodes_capacity) {
  shader_program_ = new ShaderProgram(name);
  vs_cbuffers_count_ = 0;
  ps_cbuffers_count_ = 0;
  cs_cbuffers_count_ = 0;
  texture_labels_count_ = 0;

  input_layout_ = nullptr;

  for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i) {
    vs_cbuffers_[i] = nullptr;
    ps_cbuffers_[i] = nullptr;
    cs_cbuffers_[i] = nullptr;
    vs_cbuffers_sizes_[i] = 0;
    ps_cbuffers_sizes_[i] = 0;
    cs_cbuffers_sizes_[i] = 0;
  }

  draw_nodes_capacity_ = draw_nodes_capacity;
  draw_nodes_index_ = 0;

  update_index_ = 0;
  render_index_ = 0;

}

ShaderNode::~ShaderNode() {
  if (shader_program_) {
    delete shader_program_;
    shader_program_ = nullptr;
  }

  if (input_layout_) {
    input_layout_ = nullptr;
  }

  for (int i = 0; i < MAX_NUMBER_BUFFERS; ++i) {
    /*if (vs_cbuffers_[i]) {
      delete vs_cbuffers_[i];
      vs_cbuffers_[i] = 0;
    }

    if (ps_cbuffers_[i]) {
      delete ps_cbuffers_[i];
      ps_cbuffers_[i] = 0;
    }

    if (cs_cbuffers_[i]) {
      delete cs_cbuffers_[i];
      cs_cbuffers_[i];
    }*/
  }
}

void ShaderNode::Create(const MeshResource& mesh_resource,
  ID3D11Device* device) {
  CreateInputLayout(mesh_resource, device);
  shader_program_->CreateShaders(device);
}

DrawNode& ShaderNode::GetCurrentDrawNode() {
  return draw_nodes_[update_index_][draw_nodes_index_];
}

void ShaderNode::BindDrawNodeResource(ID3D11DeviceContext* device_context, DrawNode* drawNode) {
  if (drawNode->set_view_port)
    device_context->RSSetViewports(1, &drawNode->view_port);

  // vertex buffer and index buffer
  D3D11_BUFFER_DESC desc;
  drawNode->vertex_buffer->GetDesc(&desc);
  unsigned int offset = 0;
  device_context->IASetVertexBuffers(0, 1, &drawNode->vertex_buffer,
    &desc.ByteWidth, &offset);
  device_context->IASetIndexBuffer(drawNode->index_buffer,
    DXGI_FORMAT_R32_UINT, 0);

  D3D11_MAPPED_SUBRESOURCE data;

  // update vertex shader const buffers
  for (int i = 0; i < vs_cbuffers_count_; ++i) {
    device_context->Map(vs_cbuffers_[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
    memcpy(data.pData, drawNode->vs_cbuffer_data[i], vs_cbuffers_sizes_[i]);
    device_context->Unmap(vs_cbuffers_[i], 0);
  }
  device_context->VSSetConstantBuffers(0, vs_cbuffers_count_, vs_cbuffers_);

  // update pixel shader const buffers, const buffers have unique indicies in the vertex\pixel shader
  for (int i = 0; i < ps_cbuffers_count_; ++i) {
    device_context->Map(ps_cbuffers_[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
    memcpy(data.pData, drawNode->ps_cbuffer_data[i], ps_cbuffers_sizes_[i]);
    device_context->Unmap(ps_cbuffers_[i], 0);
  }
  device_context->PSSetConstantBuffers(vs_cbuffers_count_, ps_cbuffers_count_, ps_cbuffers_);

  // textures
  device_context->PSGetShaderResources(0, drawNode->srvs_count, drawNode->srvs);

  // samplers
  device_context->PSSetSamplers(0, drawNode->samplers_count, drawNode->samplers);

}

void ShaderNode::UnbindDrawNodeResource(ID3D11DeviceContext* device_context,
  DrawNode* drawNode) {
  device_context->IASetVertexBuffers(0, 0, 0, 0, 0);
  device_context->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);

  ID3D11Buffer* buffers[MAX_NUMBER_BUFFERS] = { 0, 0 , 0, 0, 0, 0, 0, 0 };
  device_context->VSSetConstantBuffers(0, vs_cbuffers_count_, buffers);
  device_context->PSSetConstantBuffers(vs_cbuffers_count_, ps_cbuffers_count_, buffers);

  ID3D11ShaderResourceView* srvs[MAX_NUMBER_SRVS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  device_context->PSSetShaderResources(0, drawNode->srvs_count, srvs);
}

void ShaderNode::Draw(ID3D11DeviceContext* device_context) {
  BeginEvent(shader_program_->GetName().c_str());

  shader_program_->SetShaders(texture_labels_count_, texture_labels_, device_context);

  device_context->IASetInputLayout(input_layout_);

  for (DrawNode& draw_node :draw_nodes_[render_index_]) {
    BeginEvent(draw_node.name.c_str());

    BindDrawNodeResource(device_context, &draw_node);
    device_context->DrawIndexed(draw_node.primitives_count * 3, 0, 0);
    UnbindDrawNodeResource(device_context, &draw_node);

    EndEvent();
  }

  device_context->IASetInputLayout(0);

  EndEvent();
}

void ShaderNode::ClearDrawNodes() {
  for (auto& draw_node : draw_nodes_[render_index_]) {
    for (int i = 0; i < draw_node.vs_cbuffers_count; ++i) {
      draw_node.DestroyCBufferData(draw_node.vs_cbuffer_data[i]);
    }

    for (int i = 0; i < draw_node.ps_cbuffers_count; ++i) {
      draw_node.DestroyCBufferData(draw_node.ps_cbuffer_data[i]);
    }
  }

  draw_nodes_[render_index_].clear();
}

void ShaderNode::AddDrawNode(DrawNode& draw_node) {
  std::lock_guard<std::mutex> guard(draw_nodes_mutex_);
  draw_nodes_[update_index_].push_back(draw_node);
}

void ShaderNode::CreateInputLayout(const MeshResource& mesh_resource,
  ID3D11Device* device) {
  device->CreateInputLayout(mesh_resource.elements_desc,
    mesh_resource.elements_count, shader_program_->GetFileData(VERTEX_SHADER),
    shader_program_->GetFileSize(VERTEX_SHADER), &input_layout_);
}

void ShaderNode::CreateConstantBuffer(const D3D11_BUFFER_DESC& desc, ID3D11Device* device,
  ShaderType type) {
  D3D11_SUBRESOURCE_DATA data;
  data.pSysMem = nullptr;

  if (type == VERTEX_SHADER) {
    device->CreateBuffer(&desc, &data, &vs_cbuffers_[vs_cbuffers_count_]);
    vs_cbuffers_sizes_[vs_cbuffers_count_] = desc.ByteWidth;
    ++vs_cbuffers_count_;
  } else if (type == PIXEL_SHADER) {
    device->CreateBuffer(&desc, &data, &ps_cbuffers_[ps_cbuffers_count_]);
    ps_cbuffers_sizes_[ps_cbuffers_count_] = desc.ByteWidth;
    ++ps_cbuffers_count_;
  } else if (type == COMPUTE_SHADER) {
    device->CreateBuffer(&desc, &data, &cs_cbuffers_[cs_cbuffers_count_]);
    cs_cbuffers_sizes_[cs_cbuffers_count_] = desc.ByteWidth;
    ++cs_cbuffers_count_;
  }
}

void ShaderNode::LoadShader(ShaderType type, ID3D11Device* device) {
  shader_program_->LoadShader(type);
}

void ShaderNode::AddTextureLabel(int label) {
  texture_labels_[texture_labels_count_++] = label;
}

void ShaderNode::RunCompute(ID3D11DeviceContext* device_context,
  int iSRVCount, ID3D11ShaderResourceView** ppSRVs,
  void* CBufferData, int iUAVCount, ID3D11UnorderedAccessView** ppUAVs,
  unsigned int uX, unsigned int uY, unsigned uZ) {
  /*
  device_context->SetShader(COMPUTE_SHADER, shader_program_->GetShader(COMPUTE_SHADER));

  D3D11_SUBRESOURCE_Data data;
  pDeviceContext->Map(cs_cbuffers_[0], 0, D3D11_MAP_WRITE_DISCARD, 0, data);
  memcpy(data.pMem, CBufferData, cs_cbuffers_sizes_[0]);
  pDeviceContext->Unmap(cs_cbuffers_[0], 0);

  device_context->ID3D11CSSetConstantBuffers(0, cs_cbuffers_count_, COMPUTE_SHADER, cs_cbuffers_);
  device_context->SetShaderResourceViews(HALgfx::COMPUTE_SHADER, 0, iSRVCount, ppSRVs);
  device_context->SetUnorderedAccessViews(HALgfx::COMPUTE_SHADER, 0, iUAVCount, ppUAVs);

  device_context->ID3D11ExuecuteCompute(uX, uY, uZ);

  // clear
  device_context->SetShader(COMPUTE_SHADER, NULL);

  HALgfx::IBuffer* pBuffers[MAX_NUMBER_BUFFERS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  pDeviceContext->SetConstantBuffers(0, m_iNumberOfCSConstBuffers, HALgfx::COMPUTE_SHADER, pBuffers);

  HALgfx::IShaderResourceView* pSRVs[MAX_NUMBER_SRVS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  pDeviceContext->SetShaderResourceViews(HALgfx::COMPUTE_SHADER, 0, iSRVCount, pSRVs);

  HALgfx::IUnorderedAccessView* pUAVs[MAX_NUMBER_UAVS] = { 0, 0, 0, 0 };
  pDeviceContext->SetUnorderedAccessViews(HALgfx::COMPUTE_SHADER, 0, iUAVCount, pUAVs);
  */
}

void ShaderNode::SwapDoubleBuffers() {
  std::swap(render_index_, update_index_);
  draw_nodes_index_ = 0;
}

const std::string& ShaderNode::GetName() const {
  return shader_program_->GetName();
}

void ShaderNode::BeginEvent(const char* debugInfo) {
  size_t newsize = strlen(debugInfo) + 1;
  wchar_t * wcstring = new wchar_t[newsize];
  size_t convertedChars = 0;
  mbstowcs_s(&convertedChars, wcstring, newsize, debugInfo, _TRUNCATE);

  D3DPERF_BeginEvent(D3DCOLOR_XRGB(128, 128, 128), wcstring);
}

void ShaderNode::EndEvent() {
  D3DPERF_EndEvent();
}

void ShaderNode::IncreaseDrawNodeIndex() {
  draw_nodes_index_++;
}

}  // namespace render
}  // namespace magnet