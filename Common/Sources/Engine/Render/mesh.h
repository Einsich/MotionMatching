#pragma once
#include <map>
#include <assimp/scene.h>
#include "common.h"
#include "vertex_array_object.h"
class Mesh
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
public:
  map<string, int> bonesMap;//FIX IT
  Mesh(VertexArrayObject vao);
  Mesh(const aiMesh *mesh);
  void render(bool wire_frame = false);
};

using MeshPtr = shared_ptr<Mesh>;

MeshPtr make_mesh(VertexArrayObject vao);
MeshPtr make_mesh(const aiMesh *mesh);