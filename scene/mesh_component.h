#ifndef MAGNET_SCENE_MESH_COMPONENT_H_
#define UI_RENDER_TRIANGLE_MESH_COMPONENT_H_

#include <memory>
#include <string>
#include <vector>

#include "component_factory.h"
#include "icomponent.h"

namespace magnet {
namespace render {
class Mesh;
class Material;
}  // namespace render
namespace scene {

class MeshComponent : public IComponent {
 public:
  explicit MeshComponent(const std::string& name);
  ~MeshComponent();

  void Initialize() override; 
  void Update(const math::Matrix4f& local_to_world) override;
  ComponentType GetType() const override;

  void AddMesh(std::shared_ptr<render::Mesh> mesh);
  void AddMaterial(std::shared_ptr<render::Material> material);
  std::string GetName() const;

 private:
  std::string name_;

  std::vector<std::shared_ptr<render::Mesh>> meshes_;
  std::vector<std::shared_ptr<render::Material>> materials_;
};

REGISTER_COMPONENT(MeshComponent);

}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_MESH_COMPONENT_H_

