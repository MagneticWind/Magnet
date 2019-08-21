#ifndef MAGNET_RENDER_MESH_H_
#define MAGNET_RENDER_MESH_H_

#include <string>

#include "math\vector3.h"
#include "math\vector2.h"
#include "math\vector4.h"
#include "math\aabb.h"

namespace magnet {
namespace render {
enum VertexDecl
{
  POSITION,
  NORMAL,
  UV,
  COLOR,
  TANGENT,
  BINORMAL,
  MAX_DESC_COUNT
};

class Mesh {
 public:
  Mesh(const std::string& name);
  ~Mesh();

  void SetVertsCount(int verts_count);
  void SetFacesCount(int faces_count);
  int GetVertsCount() const;
  int GetFacesCount() const;
  int GetStride() const;
  void GetDecls(int* decls_count, VertexDecl* decls) const;
  const void* GetVertexDataPtr() const;
  const void* GetIndexDataPtr() const;

  bool IsLoaded() const;
  void SetLoaded(bool loaded);

  const std::string& GetName() const;
  const math::AABBf& GetBBox() const;

  void AddVertexDecl(VertexDecl eDecl);
  void GetVetexDecls(int* decls_count, VertexDecl* pDecls) const;

  float* CreateVertexDataBuffer(int iNumVertices, int iNumFloats);
  unsigned int* CreateIndexDataBuffer(int iNumFaces);

 protected:
  std::string name_;
  void* vertex_data_;
  void* index_data_;
  int verts_count_;
  int faces_count_;

  bool is_loaded_;

  int stride_;

  int decls_count_;
  VertexDecl vertex_decls_[MAX_DESC_COUNT];

  math::AABBf bbox_;
};
}  // namespace render
}  //namespace magnet

#endif
