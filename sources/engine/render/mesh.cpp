#include "mesh.h"
#include "resources/resource_registration.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "imgui.h"
#include "glad/glad.h"
#include <parallel/thread_pool.h>


void Mesh::create_vertex_array()
{
  glGenVertexArrays(1, &vertexArrayBufferObject);
  glBindVertexArray(vertexArrayBufferObject);
}

void Mesh::create_indices(const std::vector<unsigned int> &indices)
{
  GLuint arrayIndexBuffer;
  glGenBuffers(1, &arrayIndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
  numIndices = indices.size();
}

void Mesh::init_channel(int index, size_t data_size, const void *data_ptr, int component_count, bool is_float)
{
  GLuint arrayBuffer;
  glGenBuffers(1, &arrayBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
  glBufferData(GL_ARRAY_BUFFER, data_size, data_ptr, GL_STATIC_DRAW);
  glEnableVertexAttribArray(index);
  
  if (is_float) 
    glVertexAttribPointer(index, component_count, GL_FLOAT, GL_FALSE, 0, 0);
  else
    glVertexAttribIPointer(index, component_count, GL_UNSIGNED_INT, 0, 0);
}
Mesh::Mesh(const aiMesh *mesh)
{
  load_assimp(mesh);
  if (indices.size() && positions.size())
  {
    init(indices, positions, normals, uvs, weights, weightsIndex);
  }
}
uint Mesh::get_vao() const
{
  return vertexArrayBufferObject;
}

const BoundingBox &Mesh::get_bounding_box() const
{
  return box;
}

void Mesh::clear_cpu_data()
{
  indices.clear();
  normals.clear();
  positions.clear();
  uvs.clear();
  weights.clear();
  weightsIndex.clear();
}

void Mesh::load_assimp(const aiMesh *mesh)
{
  numVert = mesh->mNumVertices;
  numFaces = mesh->mNumFaces;

  if (mesh->HasPositions())
  {
    positions.resize(numVert);
    for (int i = 0; i < numVert; i++)
    {
      positions[i] = to_vec3(mesh->mVertices[i]);
      box.minimize(positions[i]);
      box.maximize(positions[i]);
    }
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
    weights.resize(numVert, vec4(0.f));
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
    //the sum of weights not 1 (((
    for (int i = 0; i < numVert; i++)
    {
      vec4 w = weights[i];
      float s = w.x + w.y + w.z + w.w;
      weights[i] *= 1.f / s;
    }
  }
}

void Mesh::load(const filesystem::path &path, bool reload, AssetStatus &status)
{
  if (reload || path.empty())
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
    if (status == AssetStatus::NotLoaded)
    {
      //debug_log("async loaded mesh %s", pathStr.c_str());
      add_job([this, pathStr, &status](){
        const string &indexedName = pathStr;
        size_t bracketPosBegin = indexedName.find_last_of('[');
        size_t bracketPosEnd = indexedName.find_last_of(']');
        if (bracketPosBegin == string::npos || bracketPosEnd == string::npos || bracketPosBegin > bracketPosEnd)
        {
          debug_error("bad mesh index in %s", indexedName.c_str());
          return;
        }
        int bracketCount = bracketPosEnd - bracketPosBegin - 1;
        int ind = std::stoi(indexedName.substr(bracketPosBegin+1, bracketCount));
        string strPath = root_path(indexedName.substr(0, bracketPosBegin));
        Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
        importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
        
        importer.ReadFile(strPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_LimitBoneWeights |
          aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);

        const aiScene* scene = importer.GetScene();
        if (!scene)
        {
          debug_error("no asset in %s", strPath.c_str());
          return;
        }
        load_assimp(scene->mMeshes[ind]);
        add_main_thread_job([this, &status]()
        {
          if (indices.size() && positions.size())
          {
            init(indices, positions, normals, uvs, weights, weightsIndex);
          }
          status = AssetStatus::Loaded;
        });
      });
    }
    status = AssetStatus::Loading;
    return;
  }
  status = AssetStatus::Loaded;
}

void Mesh::render(bool wire_frame) const
{
  glBindVertexArray(vertexArrayBufferObject);
  glDrawElementsBaseVertex(wire_frame ? GL_LINES : GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, 0);
}

void Mesh::render_instances(int instance, bool wire_frame) const
{
  glBindVertexArray(vertexArrayBufferObject);
  glDrawElementsInstancedBaseVertex(wire_frame ? GL_LINES : GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0, instance, 0);
}

bool Mesh::is_valid() const
{
  return vertexArrayBufferObject > 0 && numIndices > 0;
}

bool Mesh::edit()
{
  ImGui::Text("vertex array object: %s", is_valid() ? "is valid" : "not valid");
  return false;
}

string Mesh::asset_name(const filesystem::path &path)
{
  if (!path.parent_path().empty())
    return path.filename().string();
  else
    return path.string();
}

ResourceRegister<Mesh, true> meshregister;

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
    uvMesh = Asset<Mesh>("plane", indices, vertices, normals, uv);
    notUvMesh = Asset<Mesh>("plane (without uv)", indices, vertices, normals);
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
    uvMesh = Asset<Mesh>("cube", indices, vertices, normals, uv);
    notUvMesh = Asset<Mesh>("cube (without uv)", indices, vertices, normals);
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
      spheres[r] = Asset<Mesh>(name, indices, vertices, normals, uv);
    }
    {
      int r = detailed * (1 + 0);
      string name = "sphere " + to_string(detailed) + " (without uv)";
      spheres[r] = Asset<Mesh>(name, indices, vertices, normals);
    }
  }
  return spheres[t];  
}
