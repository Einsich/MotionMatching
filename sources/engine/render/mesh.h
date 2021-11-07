#pragma once
#include <map>
#include <assimp/scene.h>
#include "common.h"
#include "vertex_array_object.h"
#include "resources/asset.h"
#include "serialization/iserializable.h"
class Mesh : public  IAsset
{
  
  VertexArrayObject vertexArrayObject;
  int numVert;
  int numFaces;
  vector<uint> indices;
  vector<vec3> normals;
  vector<vec3> positions;
  vector<vec2> uvs;
  vector<vec4> weights;
  vector<uvec4> weightsIndex;
  void load_assimp(const aiMesh *mesh);
public:
  map<string, int> bonesMap;//FIX IT
  Mesh() = default;
  Mesh(VertexArrayObject vao);
  Mesh(const aiMesh *mesh);
  void render(bool wire_frame = false) const;
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual bool edit() override;
  virtual string asset_name(const filesystem::path &path) override;
  VertexArrayObject get_vao() const;
};


Asset<Mesh> sphere_mesh(int detailed, bool create_uv);
Asset<Mesh> cube_mesh(bool create_uv);
Asset<Mesh> plane_mesh(bool create_uv);