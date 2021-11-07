#pragma once
#include "common.h"
#include "shader/shader.h"
#include "texture/textures.h"
#include "resources/asset.h"
#include "serialization/iserializable.h"

#define TYPES \
  TYPE(float, GL_FLOAT) TYPE(vec2, GL_FLOAT_VEC2) TYPE(vec3, GL_FLOAT_VEC3) TYPE(vec4, GL_FLOAT_VEC4) \
  TYPE(int, GL_INT) TYPE(ivec2, GL_INT_VEC2) TYPE(ivec3, GL_INT_VEC3) TYPE(ivec4, GL_INT_VEC4)

#define SAMPLERS \
  SAMPLER(Asset<Texture2D>, GL_SAMPLER_2D) SAMPLER(Asset<CubeMap>, GL_SAMPLER_CUBE)

class Material : public IAsset
{
private:
  Shader shader;
  map<string, int> uniformMap;
  #define TYPE(T, _) vector<T> T##s;
  #define SAMPLER(T, gl_type) vector<T> gl_type##s;
  TYPES
  SAMPLERS
  #undef TYPE
  #undef SAMPLER
  
  #define SAMPLER(T, gl_type) (vector<pair<string, T>>) (gl_type##savable),
  #define TYPE(T, _) (vector<pair<string, vector<T>>>) (T##savable),
  REFLECT(Material,
  TYPES
  SAMPLERS
  (string) (shaderName))
  #undef SAMPLER
  #undef TYPE
  pair<int, int> get_uniform_index(const char *name, int gl_type) const;
  int get_texture_index(const char *name, int gl_type) const;
public:
  Material() = default;
  Shader &get_shader();
  const Shader &get_shader() const;
  void bind_textures_to_shader() const;
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual void before_save() override;
  virtual bool edit() override;

  uint buffer_size() const;
  void set_data_to_buf(char *data) const;
  BufferField get_buffer_field(const char *name) const;
  #define TYPE(T, gl_type)\
  bool set_property(const char *name, const T &value)\
  {\
    auto [offset, size] = get_uniform_index(name, gl_type);\
    if (offset >= 0)\
      T##s[offset] = value;\
    return offset >= 0;\
  }\
  bool set_property(const char *name, const vector<T> &value)\
  {\
    auto [offset, size] = get_uniform_index(name, gl_type);\
    if (offset >= 0)\
      for (uint i = 0, n = glm::min(size, (int)value.size()); i < n; ++i)\
        T##s[offset + i] = value[i];\
    return offset >= 0;\
  }

  #define SAMPLER(T, gl_type)\
  bool set_texture(const char *name, const T &value)\
  {\
    int offset = get_texture_index(name, gl_type);\
    if (offset >= 0)\
      gl_type##s[offset] = value;\
    return offset >= 0;\
  }

  TYPES
  SAMPLERS
  #undef TYPE
  #undef SAMPLER
  
};