#ifndef MAGNET_SCENE_SCENE_MANAGER_H_
#define MAGNET_SCENE_SCENE_MANAGER_H_

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "external\tinyxml2\tinyxml2.h"

#include "math\vector2.h"
#include "math\vector3.h"
#include "math\transformation.h"

namespace magnet {
namespace render {
class Mesh;
class Texture;
class Material;
}  // namespace render

namespace scene {

class CameraComponent;
class IEntity;
class IComponent;
class InputManager;
class Transformation;
class MeshComponent;


struct Vertex {
  math::Vector3f position;
  math::Vector3f normal;
  math::Vector2f uv;
};

struct CacheEntry {
  CacheEntry() : uv_index(-1), normal_index(-1), vertex_index(-1) {}
  int uv_index;
  int normal_index;
  int vertex_index;
};

class SceneManager {
 private:
  SceneManager();
  ~SceneManager();
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  static SceneManager* instance_;

 public:
  static void Initialize();
  static SceneManager* GetInstance();
  static void Terminate();

  //void OnWindowResize(int width, int height);

  void AddEntity(IEntity* entity);
  std::shared_ptr<render::Material> GetMaterial(std::string name);

  std::vector<IEntity*>* GetEntities();
  void GetCurrentCameraMatrix(math::Matrix4f* view, math::Matrix4f* projection);

 private:
  void SetTextureFolderPath(const std::string& folder_path);
  void SetMeshFolderPath(const std::string& folder_path);
  const std::string& GetTextureFolderPath() const;
  const std::string& GetMeshFolderPath() const;

  void LoadTexture(std::shared_ptr<render::Texture> texture);
  void LoadMeshObj(const std::string& name, MeshComponent* mesh_component);

  std::shared_ptr<render::Mesh> CreateMesh(
    const std::string& name, bool has_normal, bool has_uv,
    const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

  //  load config file
  void LoadSceneFile(const std::string& path);
  void ParseStartingPoint(tinyxml2::XMLElement* element,
    math::Vector3f* starting_point);
  void ParseEntity(tinyxml2::XMLElement* element);
  void ParseComponent(tinyxml2::XMLElement* element, IEntity* entity);
  void ParseComponent(tinyxml2::XMLElement* element,
    IComponent* current_component);
  void ParseCamera(tinyxml2::XMLElement* element, IComponent* component);
  void ParseTransformation(tinyxml2::XMLElement* element,
    math::Transformationf* transformation);
  void ParseTriangleMesh(tinyxml2::XMLElement* element, IComponent* component);
  void ParseMaterial(tinyxml2::XMLElement* element, IComponent* component);

 private:
  std::vector<IEntity*> entities_;

  std::map<std::string, CameraComponent*> cameras_;
  std::map<std::string, std::shared_ptr<render::Mesh>> meshes_;
  std::map<std::string, std::shared_ptr<render::Material>> materials_;
  std::map<std::string, std::shared_ptr<render::Texture>> textures_;

  math::Vector3f starting_point_;
  bool system_enabled_;

  std::string mesh_folder_path_;        // folder path of meshes
  std::string texture_folder_path_;     // folder path of textures
};
}  // namespace scene
}  // namespace magnet
#endif  // MAGNET_SCENE_SCENE_MANAGER_H_

