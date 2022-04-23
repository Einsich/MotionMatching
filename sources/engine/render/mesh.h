#pragma once
#include <map>
#include <assimp/scene.h>
#include "common.h"
#include "resources/asset.h"
#include "serialization/iserializable.h"
#include "occlusion.h"

class Mesh : public  IAsset
{
  uint vertexArrayBufferObject;
  int numIndices;
  int numVert;
  int numFaces;
  vector<uint> indices;
  vector<vec3> normals;
  vector<vec3> positions;
  vector<vec2> uvs;
  vector<vec4> weights;
  vector<uvec4> weightsIndex;
  BoundingBox box;
  void create_vertex_array();
  void create_indices(const std::vector<unsigned int> &indices);
  void init_channel(int index, size_t data_size, const void *data_ptr, int component_count, bool is_float);
  template<int i>
    void InitChannel() { }
  template<int i, typename T, typename... Channel>
  void InitChannel(const std::vector<T> &channel, const Channel&... channels)
  {
    if (channel.size() > 0)
    {
      const int size = sizeof(T) / sizeof(channel[0][0]);
      init_channel(i, sizeof(T) * channel.size(), channel.data(), size, !(std::is_same<T, uvec4>::value || std::is_same<T, uvec1>::value));
    }
    InitChannel<i + 1>(channels...);
  }
  void load_assimp(const aiMesh *mesh);
public:
  map<string, int> bonesMap;//FIX IT
  Mesh() = default;  
  template<typename... Channel>
  void init(const std::vector<unsigned int> &indices, const Channel&... channels)
  {
    create_vertex_array();
    InitChannel<0>(channels...);
    create_indices(indices);
  }
  template<typename... Channel>
  Mesh(const std::vector<unsigned int> &indices, const Channel&... channels)
  {
    init(indices, channels...);
  }

  Mesh(const aiMesh *mesh);
  virtual void load(const filesystem::path &path, bool reload, AssetStatus &status) override;
  virtual bool edit() override;
  virtual string asset_name(const filesystem::path &path) override;
  uint get_vao() const;
  const BoundingBox &get_bounding_box() const;
  void render(bool wire_frame = false) const;
  void render_instances(int instance, bool wire_frame = false) const;
  bool is_valid() const;
  void clear_cpu_data();
};


Asset<Mesh> sphere_mesh(int detailed, bool create_uv);
Asset<Mesh> cube_mesh(bool create_uv);
Asset<Mesh> plane_mesh(bool create_uv);