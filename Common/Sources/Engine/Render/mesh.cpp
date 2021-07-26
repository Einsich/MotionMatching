#include "mesh.h"
#include "Engine/Resources/resources.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Engine/imgui/imgui.h"

Mesh::Mesh(VertexArrayObject vao):
vertexArrayObject(vao)
{

}
Mesh::Mesh(const aiMesh *mesh)
{
  load_assimp(mesh);
}
void Mesh::load_assimp(const aiMesh *mesh)
{
  numVert = mesh->mNumVertices;
  numFaces = mesh->mNumFaces;
  if (mesh->HasPositions())
  {
    positions.resize(numVert);
    for (int i = 0; i < numVert; i++)
      positions[i] = to_vec3(mesh->mVertices[i]);
  }
  if (mesh->HasNormals())
  {
    normals.resize(numVert);
    for (int i = 0; i < numVert; i++)
      normals[i] = to_vec3(mesh->mNormals[i]);
  }
  if (mesh->HasFaces())
  {
    indices.resize(numFaces * 3);
    for (int i = 0; i < numFaces; i++)
    { 
      assert(mesh->mFaces[i].mNumIndices == 3);
      for (int j = 0; j < 3; j++)
        indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
    }
  }
  if (mesh->HasTextureCoords(0))
  {
    uvs.resize(numVert);
    for (int i = 0; i < numVert; i++)
      uvs[i] = to_vec2(mesh->mTextureCoords[0][i]);
  }
  if (mesh->HasBones())
  {
    weights.resize(numVert);
    weightsIndex.resize(numVert);
    int numBones = mesh->mNumBones;
    vector<int> weightsOffset(numVert, 0);
    for (int i = 0; i < numBones; i++)
    {
      const aiBone * bone = mesh->mBones[i];
      bonesMap[string(bone->mName.C_Str())] = i;

      for (uint j = 0; j < bone->mNumWeights; j++)
      {
        int vertex = bone->mWeights[j].mVertexId;
        int offset = weightsOffset[vertex]++;
        weights[vertex][offset] = bone->mWeights[j].mWeight;
        weightsIndex[vertex][offset] = i;
      }
    }
  }
  if (indices.size() && positions.size())
  {
    vertexArrayObject = VertexArrayObject(indices, positions, normals, uvs, weights, weightsIndex);
  }
}
void Mesh::render(bool wire_frame) const
{
  vertexArrayObject.render(wire_frame);
}
void Mesh::load(const filesystem::path &path, bool reload)
{
  if (reload)
    return;
  string pathStr = path.string();
  
  if (pathStr == "cube")
    *this = *cube_mesh(true);
  else if (pathStr == "cube (without uv)")
    *this = *cube_mesh(false);
  else if (pathStr == "plane")
    *this = *plane_mesh(true);
  else if (pathStr == "plane (without uv)")
    *this = *plane_mesh(false);
  else if (pathStr.substr(0, 6) == "sphere")
  {
    pathStr = pathStr.substr(7);
    int detailed = std::stoi(pathStr);
    bool witout_uv = pathStr.size() > 4;
    *this = *sphere_mesh(detailed, !witout_uv);
  }
  else
  {
    int ind = std::stoi(path.parent_path().filename().string());
    string strPath = path.parent_path().parent_path().string();
      Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
    
    importer.ReadFile(strPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_LimitBoneWeights |
      aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);

    const aiScene* scene = importer.GetScene();
    load_assimp(scene->mMeshes[ind]);
  }
}
void Mesh::free()
{

}
bool Mesh::edit()
{
  ImGui::Text("vertex array object: %s", vertexArrayObject.is_valid() ? "is valid" : "not valid");
  return false;
}

ResourceRegister<Mesh> meshregister;

Asset<Mesh> plane_mesh(bool create_uv)
{
  static Asset<Mesh> uvMesh;
  static Asset<Mesh> notUvMesh;
  if (!uvMesh || !notUvMesh)
  {
    vector<vec3> vertices = {vec3(-1,0,-1), vec3(1,0,-1), vec3(1,0,1), vec3(-1,0,1)};
    vector<vec3> normals(4, vec3(0,1,0));
    vector<uint> indices = {0,1,2,0,2,3};
    vector<vec2> uv =  {vec2(0,0), vec2(1,0), vec2(1,1),vec2(0,1)};
    uvMesh = Asset<Mesh>("plane", false, VertexArrayObject(indices, vertices, normals, uv));
    notUvMesh = Asset<Mesh>("plane (without uv)", false, VertexArrayObject(indices, vertices, normals));
  }
  return create_uv ? uvMesh : notUvMesh;  
}

Asset<Mesh> cube_mesh(bool create_uv)
{
  static Asset<Mesh> uvMesh;
  static Asset<Mesh> notUvMesh;
  if (!uvMesh || !notUvMesh)
  {
    vector<uint> indices;
    vector<vec3> vertices, normals;
    vector<vec2> uv;
    for (int face = 0; face < 3; face++)
    {
      for (int d = -1; d <= 1; d += 2)
      {
        vec3 norm = vec3();
        norm[face] = d;
        int ind = vertices.size();
        float a = -1, b = -1, ta, tb;
        for (int i = 0; i < 4; i++)
        {
          vec3 v;
          v[face] = d;
          v[(face + 1) % 3] = a;
          v[(face + 2) % 3] = b;

          vec2 u;
          if (face != 1)
          {
            u.x = (v.x + v.z - d + 1.f) * 0.5f;
            if ((d < 0) ^ (face == 0))
              u.x = 1.f - u.x;
            u.y = 1.f - (v.y + 1.f) * 0.5f;
          }
          else
          {
            u = (vec2(a,b) + vec2(1.f))*0.5f;
          }

          ta = -b * d;
          tb = a * d;
          a = ta;
          b = tb;
          vertices.push_back(v);
          normals.push_back(norm);
          uv.push_back(u);
        }
        indices.push_back(ind); indices.push_back(ind + 2);indices.push_back(ind + 1);
        indices.push_back(ind); indices.push_back(ind + 3); indices.push_back(ind + 2);
      }
    }
    uvMesh = Asset<Mesh>("cube", false, VertexArrayObject(indices, vertices, normals, uv));
    notUvMesh = Asset<Mesh>("cube (without uv)", false, VertexArrayObject(indices, vertices, normals));
  }
  return create_uv ? uvMesh : notUvMesh;  
}

Asset<Mesh> sphere_mesh(int detailed, bool create_uv)
{
  constexpr int maxDetailed = 20;
  detailed = glm::clamp(detailed, 1, maxDetailed);
  int t = detailed * (1 + (int)create_uv);
  static std::array<Asset<Mesh>, maxDetailed * 2 + 1> spheres;

  if (!spheres[t])
  {
    int n = detailed + 2;
    int m = detailed + 2;

    vector<uint> indices;
    vector<vec3> vertices, normals;
    vector<vec2> uv;
    for (int i = 0; i <= n; i++)
      for (int j = 0; j <= m; j++)
      {
        float u = (1.f * i / m);
        float v = (1.f * j / n);
        float b = u * PI * 2;
        float a = (v - 0.5f) * PI;
        float x = cos(a) * cos(b);
        float z = cos(a) * sin(b);
        float y = sin(a);
        vec3 p = vec3(x, y, z);
        vertices.push_back(p);
        normals.push_back(p);
        uv.push_back(vec2(u, 1.f - v));
      }
    
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
      {
        int a = i * (n + 1) + j;
        int b = a + 1;
        int c = a + n + 1;
        int d = c + 1;
        indices.push_back(a); indices.push_back(d); indices.push_back(b);
        indices.push_back(a); indices.push_back(c); indices.push_back(d);
      }
    {
      int r = detailed * (1 + 1);
      string name = "sphere " + to_string(detailed);
      spheres[r] = Asset<Mesh>(name, false, VertexArrayObject(indices, vertices, normals, uv));
    }
    {
      int r = detailed * (1 + 0);
      string name = "sphere " + to_string(detailed) + " (without uv)";
      spheres[r] = Asset<Mesh>(name, false, VertexArrayObject(indices, vertices, normals));
    }
  }
  return spheres[t];  
}
