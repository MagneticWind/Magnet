#include <string>

#include "mesh.h"

namespace magnet {
namespace render {
  Mesh::Mesh(const std::string& name) :
    name_(name),
    vertex_data_(nullptr),
    index_data_(nullptr),
    verts_count_(0),
    faces_count_(0),
    is_loaded_(false),
    stride_(0),
    decls_count_(0) {}

Mesh::~Mesh() {}

void Mesh::SetVertsCount(int verts_count) {
  verts_count_ = verts_count;
}

void Mesh::SetFacesCount(int faces_count) {
  faces_count_ = faces_count;
}

int Mesh::GetVertsCount() const {
  return verts_count_;
}

int Mesh::GetFacesCount() const {
  return faces_count_;
}

int Mesh::GetStride() const {
  return stride_;
}

void Mesh::GetDecls(int* decls_count, VertexDecl* decls) const {
  *decls_count = decls_count_;
  for (int i = 0; i < decls_count_; ++i) {
    decls[i] = vertex_decls_[i];
  }
}

const void* Mesh::GetVertexDataPtr() const {
  return vertex_data_;
}

const void* Mesh::GetIndexDataPtr() const {
  return index_data_;
}

const std::string& Mesh::GetName() const {
  return name_;
}

const math::AABBf& Mesh::GetBBox() const {
  return bbox_;
}

bool Mesh::IsLoaded() const {
  return is_loaded_;
}

void Mesh::SetLoaded(bool loaded) {
  is_loaded_ = loaded;
}

float* Mesh::CreateVertexDataBuffer(int iNumVertices, int iNumFloats) {
  stride_ = iNumFloats;
  vertex_data_ = malloc(iNumVertices * iNumFloats * sizeof(float));
  return static_cast<float*>(vertex_data_);
}

unsigned int* Mesh::CreateIndexDataBuffer(int iNumFaces) {
  index_data_ = malloc(iNumFaces * 3 * sizeof(unsigned int));
  return static_cast<unsigned int*>(index_data_);
}

void Mesh::AddVertexDecl(VertexDecl decl) {
  vertex_decls_[decls_count_++] = decl;
}

void Mesh::GetVetexDecls(int* decls_count, VertexDecl* pDecls) const {
  //TODO
}
}  // namespace scene
}  // namespace magnet