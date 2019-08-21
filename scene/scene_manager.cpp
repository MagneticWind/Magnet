#include <fstream>
#include <assert.h>
#include <fstream>
#include "external\IL\il.h"

#include "math\transformation.h"

#include "camera_component.h"
#include "component_factory.h"
#include "entity_factory.h"
#include "render\mesh.h"
#include "render\surface.h"
#include "mesh_component.h"
#include "normal_entity.h"
#include "camera_entity.h"
#include "scene_manager.h"

namespace magnet {
namespace scene {
namespace {
static const char MESH_PATH[256] = "C:\\DDrive\\projects\\magnet\\data\\mesh\\";
static const char TEXTURE_PATH[256] = "C:\\DDrive\\projects\\magnet\\data\\texture\\";
}  // namespace

SceneManager* SceneManager::instance_ = nullptr;

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

SceneManager* SceneManager::GetInstance() {
  return instance_;
}

void SceneManager::Initialize() {
  instance_ = new SceneManager();
  //instance_->LoadSceneFile("C:\\DDrive\\projects\\magnet\\data\\scene.xml");
}

void SceneManager::Terminate() {
  delete instance_;
  instance_ = nullptr;
}

std::vector<IEntity*>* SceneManager::GetEntities() {
  return &entities_;
}

void SceneManager::LoadSceneFile(const std::string& path) {
  if (path.empty()) return;

  FILE *pFile = fopen(path.c_str(), "r");
  fseek(pFile, 0, SEEK_END);
  int iSize = ftell(pFile);
  rewind(pFile);
  char* pData = new char[iSize];
  int iResult = fread(pData, 1, iSize, pFile);

  tinyxml2::XMLDocument doc;
  doc.Parse(pData);
  tinyxml2::XMLElement* scene_element = doc.FirstChildElement("scene");
  if (scene_element) {
    tinyxml2::XMLElement* child_element = scene_element->FirstChildElement();
    while (child_element) {
      std::string name(child_element->Name());
      if (name == "entity") {
        ParseEntity(child_element);
      }
      else if (name == "startingpoint") {
        ParseStartingPoint(child_element, &starting_point_);
      }
      child_element = child_element->NextSiblingElement();
    }
  }

  fclose(pFile);

  ilInit();
  for (auto texture : textures_) {
    LoadTexture(texture.second);
  }
}

void SceneManager::ParseStartingPoint(tinyxml2::XMLElement* element,
  math::Vector3f* starting_point) {

}

void SceneManager::ParseEntity(tinyxml2::XMLElement* element) {
  // attributes
  const std::string entity_name(element->Attribute("name"));
  const std::string entity_type(element->Attribute("type"));

  IEntity* entity = nullptr;
  if (entity_type == "normal") {
    entity = EntityFactory::GetInstance()->CreateEntity(
      "NormalEntity", entity_name);
  } else if (entity_type == "camera") {
    entity = EntityFactory::GetInstance()->CreateEntity(
      "CameraEntity", entity_name);
  }

  if (entity == nullptr)
    return;

  entities_.push_back(entity);

  math::Transformationf entity_transformation;
  tinyxml2::XMLElement* child_element = element->FirstChildElement();
  while (child_element) {
    const std::string kName(child_element->Name());
    if (kName == "transform") {
      ParseTransformation(child_element, &entity_transformation);
      entity->SetTransformation(entity_transformation);
    }
    else if (kName == "component") {
      ParseComponent(child_element, entity);
    }
    child_element = child_element->NextSiblingElement();
  }
  entity->Initialize();
}

void SceneManager::ParseComponent(tinyxml2::XMLElement* element, IEntity* entity) {
  const std::string kComponentName(element->Attribute("name"));
  const std::string kComponentType(element->Attribute("type"));

  if (kComponentType == "camera") {
    CameraComponent* camera_component = static_cast<CameraComponent*>(
      ComponentFactory::GetInstance()->CreateComponent(
        "CameraComponent", kComponentName));

    ParseComponent(element, camera_component);

    // for easy access

    cameras_.insert(std::pair<std::string, CameraComponent*>(
      kComponentName, camera_component));

    entity->AddComponent(camera_component);
  }
  else if (kComponentType == "mesh") {
    MeshComponent* mesh_component =
      static_cast<MeshComponent*>(
        ComponentFactory::GetInstance()->CreateComponent(
          "MeshComponent", kComponentName));
    ParseComponent(element, mesh_component);
    entity->AddComponent(mesh_component);
  }
}

void SceneManager::ParseComponent(tinyxml2::XMLElement* element,
  IComponent* current_component) {
  math::Transformationf component_transformation;
  tinyxml2::XMLElement* child_element = element->FirstChildElement();
  while (child_element) {
    const std::string kName(child_element->Name());
    if (kName == "transform") {
      ParseTransformation(child_element, &component_transformation);
      current_component->SetTransformation(component_transformation);
    }
    else if (kName == "camera") {
      ParseCamera(child_element, current_component);
    }
    else if (kName == "mesh") {
      ParseTriangleMesh(child_element, current_component);
    }
    //else if (kName == "material") {
    //  ParseMaterial(child_element, current_component);
    //}
    else if (kName == "component") {
      const std::string kComponentName(child_element->Attribute("name"));
      const std::string kComponentType(child_element->Attribute("type"));
      if (kComponentType == "camera") {
        CameraComponent* camera_component = static_cast<CameraComponent*>(
          ComponentFactory::GetInstance()->CreateComponent(
            "CameraComponent", kComponentName));

        ParseComponent(child_element, camera_component);

        // for easy access
        cameras_.insert(std::pair<std::string, CameraComponent*>(
          kComponentName, camera_component));

        current_component->AddComponent(camera_component);
      }
      else if (kComponentType == "mesh") {
        IComponent* component =
          ComponentFactory::GetInstance()->CreateComponent(
            "MeshComponent", kComponentName);
        ParseComponent(child_element, component);
        current_component->AddComponent(component);
      }
    }
    child_element = child_element->NextSiblingElement();
  }
}

void SceneManager::ParseCamera(tinyxml2::XMLElement* element, IComponent* component) {
  if (component->GetType() == IComponent::ComponentType::CAMERA) {
    CameraComponent* camera_component =
      static_cast<CameraComponent*>(component);

    const std::string kType(element->Attribute("type"));
    if (kType == "free") {
      camera_component->SetCameraType(CameraComponent::CameraType::FREE);
    }
    else if (kType == "firstperson") {
      camera_component->SetCameraType(
        CameraComponent::CameraType::FIRST_PERSON);
    }
    else if (kType == "thirdperson") {
      camera_component->SetCameraType(
        CameraComponent::CameraType::THIRD_PERSON);
    }
    else if (kType == "thirdpersonfixed") {
      camera_component->SetCameraType(
        CameraComponent::CameraType::THIRD_PERSON_FIXED);
    }
    else if (kType == "topview") {
      camera_component->SetCameraType(CameraComponent::CameraType::TOP_VIEW);
    }
    else if (kType == "topviewfixed") {
      camera_component->SetCameraType(
        CameraComponent::CameraType::TOP_VIEW_FIXED);
    }

    tinyxml2::XMLElement* child_element = element->FirstChildElement();
    while (child_element) {
      const std::string kName(child_element->Name());
      if (kName == "lookat") {
        const char* kLookat = child_element->GetText();
        math::Vector3f lookat;
        sscanf(kLookat, "%f %f %f", &lookat.x_, &lookat.y_, &lookat.z_);
        camera_component->SetLocalLookat(lookat);
      }
      else if (kName == "fov") {
        float fov;
        child_element->QueryFloatText(&fov);
        camera_component->SetFov(fov);
      }
      else if (kName == "aspectratio") {
        float aspect_ratio;
        child_element->QueryFloatText(&aspect_ratio);
        camera_component->SetAspectRatio(aspect_ratio);
      }
      else if (kName == "up") {
        const char* kUp = child_element->GetText();
        math::Vector3f up;
        sscanf(kUp, "%f %f %f", &up.x_, &up.y_, &up.z_);
        camera_component->SetUp(up);
      }
      child_element = child_element->NextSiblingElement();
    }
  }
}

void SceneManager::ParseTransformation(tinyxml2::XMLElement* element,
  math::Transformationf* transformation) {
  tinyxml2::XMLElement* child_element = element->FirstChildElement();
  while (child_element) {
    const std::string kName(child_element->Name());
    if (kName == "translate") {
      const char* kTranslate = child_element->GetText();
      math::Vector3f translate;
      sscanf(kTranslate, "%f %f %f", &translate.x_, &translate.y_, &translate.z_);

      transformation->SetTranslation(translate);
    }
    else if (kName == "scale") {
      const char* kScale = child_element->GetText();
      math::Vector3f scale;
      sscanf(kScale, "%f %f %f", &scale.x_, &scale.y_, &scale.z_);

      transformation->SetScale(scale);
    }
    else if (kName == "rotate") {
      const char* kRotate = child_element->GetText();
      math::Vector3f rotate;
      sscanf(kRotate, "%f %f %f", &rotate.x_, &rotate.y_, &rotate.z_);

      transformation->Rotate(rotate.x_, 1.f, 0.f, 0.f);
      transformation->Rotate(rotate.y_, 0.f, 1.f, 0.f);
      transformation->Rotate(rotate.z_, 0.f, 0.f, 1.f);
    }
    child_element = child_element->NextSiblingElement();
  }
}

void SceneManager::ParseTriangleMesh(tinyxml2::XMLElement* element, IComponent* component) {
  if (component->GetType() == IComponent::ComponentType::MESH) {
    MeshComponent* mesh_component = static_cast<MeshComponent*>(component);

    const std::string kMeshType(element->Attribute("type"));
    const std::string kMeshName(element->GetText());
    auto iter = meshes_.find(kMeshName);
    if (iter == meshes_.end()) {
      if (kMeshType == "obj") {
        LoadMeshObj(kMeshName, mesh_component);
      }
    }
    else {
      mesh_component->AddMesh(iter->second);
    }
  }
}

void ParseMaterial(tinyxml2::XMLElement* element, IComponent* component) {

}

void SceneManager::LoadTexture(std::shared_ptr<render::Texture> texture) {
  if (texture->GetType() == render::TEXTURE_TYPE_2D) {
    std::string path = std::string(TEXTURE_PATH) + texture->GetName();

    // load images using IL
    unsigned int uTextureID;
    ilGenImages(1, &uTextureID);
    ilBindImage(uTextureID);

    // the formatting setting influences of Texture loading, so must be set up before loading
    ilEnable(IL_FORMAT_SET);
    ilSetInteger(IL_FORMAT_MODE, IL_RGBA); // assume all textures are rgba

    if (!ilLoadImage(path.c_str())) {
      return;
    }

    int iWidth = ilGetInteger(IL_IMAGE_WIDTH);
    int iHeight = ilGetInteger(IL_IMAGE_HEIGHT);

    texture->SetDimension(iWidth, iHeight);

    void* pData = texture->CreateDataBuffer();

    if (texture->GetFormat() == render::TEXTURE_FORMAT_R8G8B8A8_UINT) {
      assert(ilGetInteger(IL_IMAGE_TYPE) == IL_UNSIGNED_BYTE);

      unsigned char* pDataUINT8 = static_cast<unsigned char*>(pData);
      unsigned char* pImageData = ilGetData();
      for (int y = 0; y < iHeight; y++) {
        for (int x = 0; x < iWidth; x++) {
          int i = (y * iWidth + x) * 4;
          pDataUINT8[i] = pImageData[i];
          pDataUINT8[i + 1] = pImageData[i + 1];
          pDataUINT8[i + 2] = pImageData[i + 2];
          pDataUINT8[i + 3] = pImageData[i + 3];
        }
      }
    }

    ilDeleteImage(uTextureID);
  }
  // load 6 cube map textures
  else if (texture->GetType() == render::TEXTURE_TYPE_CUBE) {
    float* pData = 0;

    char caPath[256];
    for (int i = 0; i < 6; ++i) {
      strcpy(caPath, TEXTURE_PATH);

      char caName[256];
      strcpy(caName, texture->GetName().c_str());
      int length = strlen(caName);
      char caPostfix[3] = { caName[length - 3], caName[length - 2], caName[length - 1] };
      length -= 4;
      caName[length++] = '_';
      caName[length++] = 'c';
      caName[length++] = '0';

      char cIndex[10];
      itoa(i, cIndex, 10);
      caName[length++] = cIndex[0];
      caName[length++] = '.';
      caName[length++] = caPostfix[0];
      caName[length++] = caPostfix[1];
      caName[length++] = caPostfix[2];
      caName[length] = '\0';

      strcat(caPath, caName);

      // load images using IL
      unsigned int uTextureID;
      ilGenImages(1, &uTextureID);
      ilBindImage(uTextureID);

      // the formatting setting influences of Texture loading, so must be set up before loading
      ilEnable(IL_FORMAT_SET);
      ilSetInteger(IL_FORMAT_MODE, IL_RGBA); // assume all textures are rgba

      if (!ilLoadImage(caPath)) {
        assert(0);
      }

      int w = ilGetInteger(IL_IMAGE_WIDTH);
      int h = ilGetInteger(IL_IMAGE_HEIGHT);

      if (i == 0) {
        texture->SetDimension(w, h);
        pData = static_cast<float*>(texture->CreateDataBuffer());
      }

      if (texture->GetFormat() == render::TEXTURE_FORMAT_R32G32B32A32_FLOAT) {
        assert(ilGetInteger(IL_IMAGE_TYPE) == IL_FLOAT);

        float* pDataOffset = pData + i * w * h * 4;

        float* pImageData = reinterpret_cast<float*>(ilGetData());
        for (int y = 0; y < h; y++) {
          for (int x = 0; x < w; x++) {
            int offset = (y * w + x) * 4;
            pDataOffset[offset] = pImageData[offset];
            pDataOffset[offset + 1] = pImageData[offset + 1];
            pDataOffset[offset + 2] = pImageData[offset + 2];
            pDataOffset[offset + 3] = pImageData[offset + 3];
          }
        }

      }

      ilDeleteImage(uTextureID);
    }
  }
}

std::shared_ptr<render::Mesh> SceneManager::CreateMesh(
  const std::string& name, bool has_normal, bool has_uv,
  const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
  std::shared_ptr<render::Mesh> mesh = std::make_shared<render::Mesh>(name);

  int floats_count = 0;
  if (vertices.size() > 0) {
    mesh->AddVertexDecl(render::VertexDecl::POSITION);
    floats_count += 3;
  }

  if (has_normal) {
    mesh->AddVertexDecl(render::VertexDecl::NORMAL);
    floats_count += 3;
  }

  if (has_uv) {
    mesh->AddVertexDecl(render::VertexDecl::UV);
    floats_count += 2;
  }

  float* vertex_data_buffer =
    mesh->CreateVertexDataBuffer(vertices.size(), floats_count);
  for (int i = 0; i < vertices.size(); ++i) {
    const Vertex& vertex = vertices[i];

    int index = 0;
    if (vertices.size() > 0) {
      vertex_data_buffer[i * floats_count + index] = vertex.position.x_;
      vertex_data_buffer[i * floats_count + index + 1] = vertex.position.y_;
      vertex_data_buffer[i * floats_count + index + 2] = vertex.position.z_;

      index += 3;
    }

    if (has_normal) {
      vertex_data_buffer[i * floats_count + index] = vertex.normal.x_;
      vertex_data_buffer[i * floats_count + index + 1] = vertex.normal.y_;
      vertex_data_buffer[i * floats_count + index + 2] = vertex.normal.z_;

      index += 3;
    }

    if (has_uv) {
      vertex_data_buffer[i * floats_count + index] = vertex.uv.x_;
      vertex_data_buffer[i * floats_count + index + 1] = vertex.uv.y_;

      index += 2;
    }
  }
  mesh->SetVertsCount(vertices.size());
  mesh->SetFacesCount(indices.size() / 3);
  unsigned int* index_data_buffer =
    mesh->CreateIndexDataBuffer(indices.size() / 3);
  for (int i = 0; i < indices.size(); ++i) {
    index_data_buffer[i] = indices[i];
  }

  //mesh->CreateVBO();
  return mesh;
}

void SceneManager::LoadMeshObj(const std::string& name, MeshComponent* mesh_component) {
  std::string file_path = std::string(MESH_PATH) + name;

  std::ifstream in_file(file_path);

  if (!in_file.is_open()) {
    return;
  }

  std::vector<math::Vector3f> positions;
  std::vector<math::Vector2f> uvs;
  std::vector<math::Vector3f> normals;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::map<int, std::list<CacheEntry>> cache;

  std::string material_file = "";

  char word[32];
  char mesh_name[64];

  bool is_parsing_faces = false;

  while (1) {
    in_file >> word;

    if (!in_file) {
      break;
    }

    if (strcmp(word, "#") == 0) {
      // comment
    }
    else if (strcmp(word, "o") == 0) {
      // object name
      in_file >> mesh_name;
    }
    else if (strcmp(word, "v") == 0) {
      // parsing f to parsing v, create a mesh
      if (is_parsing_faces) {
        const std::string kMeshName(mesh_name);
        std::shared_ptr<render::Mesh> mesh = CreateMesh(
          kMeshName, normals.size() > 0, uvs.size() > 0, vertices, indices);

        meshes_.insert(std::pair<std::string, std::shared_ptr<render::Mesh>>(
          kMeshName, mesh));
        mesh_component->AddMesh(mesh);

        vertices.clear();
        indices.clear();
        cache.clear();

        is_parsing_faces = false;
      }

      // parse vertex
      float x, y, z;
      in_file >> x >> y >> z;
      positions.push_back(math::Vector3f(x, y, z));
    }
    else if (strcmp(word, "vt") == 0) {
      // uv
      float u, v;
      in_file >> u >> v;
      uvs.push_back(math::Vector2f(u, 1 - v));
    }
    else if (strcmp(word, "vn") == 0) {
      // normal
      float x, y, z;
      in_file >> x >> y >> z;
      normals.push_back(math::Vector3f(x, y, z));
    }
    else if (strcmp(word, "f") == 0) {
      is_parsing_faces = true;

      // face/triangle
      int index_position, index_uv, index_normal;
      Vertex vertex;

      for (int i = 0; i < 3; ++i) {
        in_file >> index_position;
        vertex.position = positions[index_position - 1];

        if ('/' == in_file.peek()) {
          in_file.ignore();

          if ('/' != in_file.peek()) {
            in_file >> index_uv;
            vertex.uv = uvs[index_uv - 1];
          }

          if ('/' == in_file.peek()) {
            in_file.ignore();

            in_file >> index_normal;
            vertex.normal = normals[index_normal - 1];
          }
        }

        //  find in the cache
        auto it = cache.find(index_position);
        if (it != cache.end()) {
          //  search the entries
          bool found_it = false;
          std::list<CacheEntry>& list = it->second;
          for (auto it_list : list) {
            //  found it
            if (it_list.uv_index == index_uv &&
              it_list.normal_index == index_normal) {
              found_it = true;
              indices.push_back(it_list.vertex_index);
              break;
            }
          }

          if (!found_it) {
            //  add this vertex
            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);

            //  add cache entry in the list
            CacheEntry entry;
            entry.normal_index = index_normal;
            entry.uv_index = index_uv;
            entry.vertex_index = vertices.size() - 1;

            it->second.push_back(entry);
          }
        }
        else {
          // add the vertex
          vertices.push_back(vertex);
          indices.push_back(vertices.size() - 1);

          std::list<CacheEntry> list;

          // add cache entry
          CacheEntry entry;
          entry.normal_index = index_normal;
          entry.uv_index = index_uv;
          entry.vertex_index = vertices.size() - 1;

          list.push_back(entry);

          cache.insert(std::pair<int, std::list<CacheEntry>>(index_position, list));
        }
      }
    }
    else if (strcmp(word, "mtllib") == 0) {
      in_file >> word;

      const std::string kMaterialFilePath =
        std::string(MESH_PATH) + std::string(word);

      // load materials from the material file
      std::ifstream material_filestream(kMaterialFilePath);

      if (!material_filestream) break;

      std::shared_ptr<render::Material> material;

      math::Vector3f base_color;
      std::string texture_name;
      bool has_color = false;
      bool has_texture = false;
      bool first_material = true;
      char material_name[64];

      while (1) {
        material_filestream >> word;

        if (!material_filestream) break;

        // create a new material for current instance
        if (strcmp(word, "newmtl") == 0) {
          if (first_material) {
            material_filestream >> material_name;
            first_material = false;
            continue;
          }

          //  create last/current material
          if (has_color && has_texture) {
            material = std::make_shared<render::Material>();
            /*material->SetShaderType(Shader::ShaderType::TEXTURE_PNT);
            material->SetColorTextureName(std::string(kTexturePath) +
              texture_name);
            material->SetBaseColor(
              QVector4D(base_color.x(), base_color.y(), base_color.z(), 1.f));
            material->Initialize();*/
            materials_.insert(std::pair<std::string, std::shared_ptr<render::Material>>(
              std::string(material_name), material));
          }
          else if (has_color && !has_texture) {
            material = std::make_shared<render::Material>();
            /*material->SetShaderType(Shader::ShaderType::COLOR_PNT);
            material->SetBaseColor(
              QVector4D(base_color.x(), base_color.y(), base_color.z(), 1.f));
            material->Initialize();*/
            materials_.insert(std::pair<std::string, std::shared_ptr<render::Material>>(
              std::string(material_name), material));
          }

          material_filestream >> material_name;

          material = nullptr;
          has_texture = false;
          texture_name = "";
          has_color = false;
        }
        else if (strcmp(word, "Kd") == 0) {
          float r, g, b;
          material_filestream >> r >> g >> b;
          base_color = math::Vector3f(r, g, b);
          has_color = true;
        }
        else if (strcmp(word, "Ka") == 0) {
        }
        else if (strcmp(word, "Ks") == 0) {
        }
        else if (strcmp(word, "map_Kd") == 0) {
          material_filestream >> word;
          texture_name = std::string(word);
          has_texture = true;

          auto it = textures_.find(texture_name);
          if (it == textures_.end()) {
            std::shared_ptr<render::Texture> texture = std::make_shared<render::Texture>(texture_name);
            textures_.insert(std::pair<std::string, std::shared_ptr<render::Texture>>(
              texture_name, texture));
          }
        }
        else if (strcmp(word, "map_Ka") == 0) {
        }
        else if (strcmp(word, "map_Ks") == 0) {
        }
        else if (strcmp(word, "Ns") == 0) {
        }
        else if (strcmp(word, "illum") == 0) {
        }

        material_filestream.ignore(256, '\n');
      }

      // last material of the file
      if (has_color && has_texture) {
        material = std::make_shared<render::Material>();
        /*material->SetShaderType(Shader::ShaderType::TEXTURE_PNT);
        material->SetColorTextureName(std::string(kTexturePath) + texture_name);
        material->SetBaseColor(
          QVector4D(base_color.x(), base_color.y(), base_color.z(), 1.f));
        material->Initialize();*/
        materials_.insert(std::pair<std::string, std::shared_ptr<render::Material>>(
          std::string(material_name), material));
       }
      else if (has_color && !has_texture) {
        material = std::make_shared<render::Material>();
        /*material->SetShaderType(Shader::ShaderType::COLOR_PNT);
        material->SetBaseColor(
          QVector4D(base_color.x(), base_color.y(), base_color.z(), 1.f));
        material->Initialize();*/
        materials_.insert(std::pair<std::string, std::shared_ptr<render::Material>>(
          std::string(material_name), material));
      }
    }
    else if (strcmp(word, "g") == 0) {
      // group name
      // in_file >> mesh_name;
    }
    else if (strcmp(word, "usemtl") == 0) {
      in_file >> word;

      auto it = materials_.find(std::string(word));
      if (it != materials_.end()) {
        mesh_component->AddMaterial(it->second);
      }

      if (is_parsing_faces) {
        const std::string kMeshName("meshpiece");
        std::shared_ptr<render::Mesh> mesh = CreateMesh(
          kMeshName, normals.size() > 0, uvs.size() > 0, vertices, indices);

        vertices.clear();
        indices.clear();
        cache.clear();

        meshes_.insert(std::pair<std::string, std::shared_ptr<render::Mesh>>(
          kMeshName, mesh));
        mesh_component->AddMesh(mesh);
      }
    }

    in_file.ignore(256, '\n');
  }

  // parsing f to file end
  if (is_parsing_faces) {
    const std::string kMeshName(mesh_name);
    std::shared_ptr<render::Mesh> mesh = CreateMesh(
      kMeshName, normals.size() > 0, uvs.size() > 0, vertices, indices);

    meshes_.insert(
      std::pair<std::string, std::shared_ptr<render::Mesh>>(kMeshName, mesh));
    mesh_component->AddMesh(mesh);

    vertices.clear();
    indices.clear();
    cache.clear();

    is_parsing_faces = false;
  }
}

void SceneManager::GetCurrentCameraMatrix(math::Matrix4f* view, math::Matrix4f* projection) {
  auto it = cameras_.find("current");
  if (it != cameras_.end()) {
    it->second->GetViewProjectionMatrix(view, projection);
  }
}

}  // namespace scene
}  // namespace magnet