#ifndef MAGNET_SCENE_SURFACE_H_
#define MAGNET_SCENE_SURFACE_H_

#include <memory>

#include "math\aabb.h"
#include "math\matrix4.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"

namespace magnet {
namespace render {
class Surface {
 public:
  Surface() {}
  ~Surface() {};

  const math::AABBf& GetBBox() const;
  std::shared_ptr<Material> GetMaterial();
  std::shared_ptr<Mesh> GetMesh();
  math::Matrix4f GetWorld() const;

  void SetMaterial(std::shared_ptr<Material> material);
  void SetMesh(std::shared_ptr<Mesh> mesh);
  void SetWorld(const math::Matrix4f& world);

private:
  std::shared_ptr<Mesh> mesh_;
  std::shared_ptr<Material> material_;
  math::Matrix4f world_;
  math::AABBf bbox_;
};

inline const math::AABBf& Surface::GetBBox() const {
  return bbox_;
}

inline std::shared_ptr<Material> Surface::GetMaterial() {
  return material_;
}

inline std::shared_ptr<Mesh> Surface::GetMesh() {
  return mesh_;
}

inline math::Matrix4f Surface::GetWorld() const {
  return world_;
}

inline void Surface::SetMaterial(std::shared_ptr<Material> material) {
  material_ = material;
}

inline void Surface::SetMesh(std::shared_ptr<Mesh> mesh) {
  mesh_ = mesh;
}

inline void Surface::SetWorld(const math::Matrix4f& world) {
  world_ = world;
}

}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_SURFACE_H_
