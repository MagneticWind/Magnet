#ifndef MAGNET_RENDER_SHADER_NODE_H_
#define MAGNET_RENDER_SHADER_NODE_H_

#include <vector>
#include <d3d11.h>
#include <mutex>

#include "shader.h"
#include "draw_node.h"
#include "gpu_resource.h"

namespace magnet {
namespace scene {
class Surface;
class Texture;
}  // namespace scene

namespace render {
class Shader;
struct DrawNode;

class ShaderNode {
 public:
  ShaderNode() = delete;
  ShaderNode(const std::string& name, int draw_nodes_capacity);
  ~ShaderNode();

  void BindDrawNodeResource(ID3D11DeviceContext* device_context,
    DrawNode* drawNode);
  void UnbindDrawNodeResource(ID3D11DeviceContext* device_context,
    DrawNode* draw_node);
  void Draw(ID3D11DeviceContext* device_context);
  void ClearDrawNodes();
  void AddDrawNode(DrawNode& draw_node);
  void CreateConstantBuffer(const D3D11_BUFFER_DESC& desc,
    ID3D11Device* device,
    ShaderType type);

  void SwapDoubleBuffers();

  const std::string& GetName() const;

  // load compiled shades, create cbuffer, input layout and such
  void LoadShader(ShaderType eType, ID3D11Device* device);
  void Create(const MeshResource& mesh_resource,
    ID3D11Device* device);

  void* CreateBuffer(int size, ShaderType type);

  DrawNode& GetCurrentDrawNode();

  void AddTextureLabel(int label);

  static void BeginEvent(const char* debug_info);
  static void EndEvent();

  void IncreaseDrawNodeIndex();

  // compute shader
  void RunCompute(ID3D11DeviceContext* device_context, int iSRVCount,
    ID3D11ShaderResourceView** ppSRVs, void* CBufferData, int iUAVCount,
    ID3D11UnorderedAccessView** pUAVs,
    unsigned int x, unsigned int y, unsigned z);

 private:
  void CreateInputLayout(const MeshResource& mesh_resource,
     ID3D11Device* device);

 private:
  // shader program
  ShaderProgram* shader_program_;

  // input layout
  ID3D11InputLayout* input_layout_;

  // const buffers
  ID3D11Buffer* vs_cbuffers_[MAX_NUMBER_BUFFERS];
  ID3D11Buffer* ps_cbuffers_[MAX_NUMBER_BUFFERS];
  ID3D11Buffer* cs_cbuffers_[MAX_NUMBER_BUFFERS];
  int vs_cbuffers_sizes_[MAX_NUMBER_BUFFERS];
  int ps_cbuffers_sizes_[MAX_NUMBER_BUFFERS];
  int cs_cbuffers_sizes_[MAX_NUMBER_BUFFERS];
  int vs_cbuffers_count_;
  int ps_cbuffers_count_;
  int cs_cbuffers_count_;

  // texture labels
  int texture_labels_count_;
  int texture_labels_[MAX_NUMBER_SRVS];

  // use vector so that it's easier to sort if needed
  // double buffered for multithreading
  int draw_nodes_index_;
  int draw_nodes_capacity_;
  std::vector<DrawNode> draw_nodes_[2];
  int update_index_;
  int render_index_;

  // used by mutiple threads(including render thread)
  std::mutex draw_nodes_mutex_;
};

}  // namespace render
}  // namespace magnet
#endif  // MAGNET_RENDER_SHADER_NODE_H_
