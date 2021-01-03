#pragma once
#include "CommonCode/common.h"
#include "CommonCode/Mesh/mesh.h"
#include "CommonCode/Shader/shader.h"
#include "CommonCode/Material/material.h"
#include "CommonCode/Transform/transform.h"
#include "CommonCode/Camera/camera.h"
#include "CommonCode/Light/direction_light.h"
class GameObject
{
private:
  Transform transform;
  MeshPtr mesh;
  MaterialPtr material;
  Shader shader;

public:
  GameObject(const Transform &transform, MeshPtr mesh_ptr, MaterialPtr materail_ptr, const Shader& shader):
    transform(transform), mesh(mesh_ptr), material(materail_ptr), shader(shader)
  {
    
  }
  Transform &get_transform();
  void render(const Camera& mainCam, const DirectionLight& light);
  MaterialPtr get_material();
  MeshPtr get_mesh();
  void set_shader(const Shader & shader);
  Shader& get_shader();
};

using GameObjectPtr = shared_ptr<GameObject>;
GameObjectPtr make_game_object(const Transform& transform, MeshPtr mesh_ptr, MaterialPtr material_ptr, const Shader& shader);

GameObjectPtr create_plane(const Transform &transform, bool create_uv = false);
GameObjectPtr create_cube(const Transform &transform, bool create_uv = false);
GameObjectPtr create_sphere(const Transform &transform, bool create_uv = false);