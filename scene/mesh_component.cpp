#include "render/material.h"
#include "render/mesh.h"
#include "render/render_manager.h"
#include "mesh_component.h"

namespace magnet {
namespace scene {

MeshComponent::MeshComponent(const std::string& name)
  : name_(name) {
}

MeshComponent::~MeshComponent() {
  for (auto mesh : meshes_) mesh = nullptr;

  for (auto material : materials_) material = nullptr;
}

void MeshComponent::Initialize() {

}

void MeshComponent::Update(const math::Matrix4f& local_to_world) {

  math::Matrix4f world = local_to_world * transformation_.ToMatrix();

  int index = 0;
  for (auto mesh : meshes_) {
    render::Surface surface;
    surface.SetMesh(mesh);
    surface.SetMaterial(materials_[index]);
    surface.SetWorld(world);
    ++index;

	  render::RenderManager::GetInstance()->Update(&surface);
  }

  for (IComponent* component : child_components_)
    component->Update(world);
}

void MeshComponent::AddMesh(std::shared_ptr<render::Mesh> triangle_mesh) {
  meshes_.push_back(triangle_mesh);
}

void MeshComponent::AddMaterial(std::shared_ptr<render::Material> material) {
  materials_.push_back(material);
}

IComponent::ComponentType MeshComponent::GetType() const {
  return IComponent::ComponentType::MESH;
}

std::string MeshComponent::GetName() const { return name_; }

}  // namespace scene
}  // namespace magnet

