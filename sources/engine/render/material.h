#pragma once
#include "common.h"
#include "shader/shader.h"
#include "texture/textures.h"
#include "resources/asset.h"
#include "serialization/iserializable.h"

#define TYPES \
  TYPE(float, GL_FLOAT) TYPE(vec2, GL_FLOAT_VEC2) TYPE(vec3, GL_FLOAT_VEC3) TYPE(vec4, GL_FLOAT_VEC4) \
  TYPE(int, GL_INT) TYPE(ivec2, GL_INT_VEC2) TYPE(ivec3, GL_INT_VEC3) TYPE(ivec4, GL_INT_VEC4)\
  TYPE(uint, GL_UNSIGNED_INT) TYPE(uvec2, GL_UNSIGNED_INT_VEC2) TYPE(uvec3, GL_UNSIGNED_INT_VEC3) TYPE(uvec4, GL_UNSIGNED_INT_VEC4)

#define SAMPLERS \
  SAMPLER(Asset<Texture2D>, GL_SAMPLER_2D)\
  SAMPLER(Asset<CubeMap>, GL_SAMPLER_CUBE)\
  SAMPLER(Asset<Texture2DArray>, GL_SAMPLER_2D_ARRAY)\

class Material : public IAsset
{
private:
  Shader shader;
  std::map<std::string, int> uniformMap;
  #define TYPE(T, _) std::vector<T> T##s;
  #define SAMPLER(T, gl_type) std::vector<T> gl_type##s;
  TYPES
  SAMPLERS
  #undef TYPE
  #undef SAMPLER
  
  #define SAMPLER(T, gl_type) (std::vector<std::pair<std::string, T>>) (gl_type##savable),
  #define TYPE(T, _) (std::vector<std::pair<std::string, std::vector<T>>>) (T##savable),
  REFLECT(Material,
  TYPES
  SAMPLERS
  (std::string) (shaderName),
  (bool) (isTransparent),
  (bool) (disableZTest),
  (int) (drawOrder))
  #undef SAMPLER
  #undef TYPE
  std::pair<int, int> get_uniform_index(const char *name, int gl_type) const;
  int get_texture_index(const char *name, int gl_type) const;
public:
  Material() = default;
  Shader &get_shader();
  const Shader &get_shader() const;
  void bind_textures_to_shader() const;
  bool is_transparent() const
  {
    return isTransparent;
  }
  bool need_z_test() const
  {
    return !disableZTest;
  }
  int draw_order() const
  {
    return drawOrder;
  }
  virtual void load(const std::filesystem::path &path, bool reload, AssetStatus &status) override;
  
  virtual bool require_immediate_load() const override
  {
    return false;
  }
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
  bool set_property(const char *name, const std::vector<T> &value)\
  {\
    auto [offset, size] = get_uniform_index(name, gl_type);\
    if (offset >= 0)\
      for (uint i = 0, n = glm::min(size, (int)value.size()); i < n; ++i)\
        T##s[offset + i] = value[i];\
    return offset >= 0;\
  }\
  bool set_property(const char *name, const eastl::vector<T> &value)\
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