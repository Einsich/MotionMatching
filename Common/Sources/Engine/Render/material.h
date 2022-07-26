#pragma once
#include "common.h"
#include "Shader/shader.h"
#include "Texture/textures.h"
#include "Engine/Resources/asset.h"
#include "Serialization/iserializable.h"
class Property final: public ISerializable
{
private:
  vec4 property;
  Asset<Texture2D> texture;
  string name;
  int vecType;
public:
  Property() = default;
  Property(const string& name, Asset<Texture2D> property)
    :property(vec4()), texture(property), name(name) {vecType = 5;}
  Property(const string& name, vec4 property)
    :property(property), texture(), name(name) {vecType = 4;}
  Property(const string& name, vec3 property)
    :Property(name, vec4(property, 0)) {vecType = 3;}
  Property(const string& name, vec2 property)
    :Property(name, vec3(property, 0)) {vecType = 2;}
  Property(const string& name, float property)
    :Property(name, vec2(property, 0)) {vecType = 1;}
  void bind_to_shader(const Shader &shader) const;
  void unbind_to_shader(const Shader &shader) const;
  bool operator== (const Property & other) const;
  static bool edit_property(Property& property);
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};

class Material;

struct MaterialProperties
{
  friend Material;
private:
  struct Uniform
  {
    uint type, size, offset, sizeOf;
  };
  #define TYPES \
  TYPE(float, GL_FLOAT) TYPE(vec2, GL_FLOAT_VEC2) TYPE(vec3, GL_FLOAT_VEC3) TYPE(vec4, GL_FLOAT_VEC4) \
  TYPE(int, GL_INT) TYPE(ivec2, GL_INT_VEC2) TYPE(ivec3, GL_INT_VEC3) TYPE(ivec4, GL_INT_VEC4)\
  TYPE(mat2, GL_FLOAT_MAT2) TYPE(mat3, GL_FLOAT_MAT3) TYPE(mat4, GL_FLOAT_MAT4) 
//TYPE(uint, GL_UNSIGNED_INT) TYPE(uvec2, GL_UNSIGNED_INT_VEC2) TYPE(uvec3, GL_UNSIGNED_INT_VEC3) TYPE(uvec4, GL_UNSIGNED_INT_VEC4)

  #define TYPE(T, gl_name) (std::vector<T>) (T##s),

  REFLECT(MaterialProperties,
    TYPES
    (vector<Uniform>) (uniforms),
    (map<string, int>) (uniformMap),
    (uint) (bufSize)
  )
  
  #undef TYPE
private:
  int get_uniform_index(const char *name) const
  {
    auto it = uniformMap.find(name);
    return it != uniformMap.end() ? it->second : -1;
  }
  char *next_data(char *data, uint sizeOf, uint size) const
  {
    if (size == 1)
    {
      if (sizeOf <= 4)
        data += 4;
      else if (sizeOf <= 8)
        data += 8;
      else if (sizeOf <= 16)
        data += 16;
      else
        data += sizeOf;
    }
    else
      data += glm::max(16u, sizeOf);
    return data;
  }
public:
  void add_uniform(uint size, uint type, const char *name)
  {  
    #define TYPE(T, gl_name) case gl_name : offset = T##s.size(); T##s.resize(T##s.size() + size); sizeOf = sizeof(T); break;
  
    uint offset = 0, sizeOf = 0;
    switch (type)
    {
      TYPES
      default: break;
    }
    if (size == 1)
    {
      if (sizeOf <= 4)
        bufSize += 4;
      else if (sizeOf <= 8)
        bufSize += 8;
      else if (sizeOf <= 16)
        bufSize += 16;
      else
        bufSize += sizeOf;
    }
    else
      bufSize += glm::max(16u, sizeOf) * size;
    uniformMap.emplace(name, uniforms.size());
    uniforms.emplace_back(Uniform{type, size, offset, sizeOf});
    #undef TYPE
  }
  #define TYPE(T, _)\
  bool set_property(const char *name, const T &value)\
  {\
    int i = get_uniform_index(name);\
    if (i >= 0)\
      T##s[uniforms[i].offset] = value;\
    return i >= 0;\
  }\
  bool set_property(const char *name, const vector<T> &value)\
  {\
    int i = get_uniform_index(name);\
    if (i >= 0)\
      for (uint i = 0, n = glm::min(uniforms[i].size, (uint)value.size()); i < n; ++i)\
        T##s[uniforms[i].offset + i] = value[i];\
    return i >= 0;\
  }
  
  TYPES
  #undef TYPE


  void set_data_to_buf(char *data) const
  {
    for (const Uniform &uniform : uniforms)
    {
      uint size = uniform.size;
      uint sizeOf = uniform.sizeOf;
      switch (uniform.type)
      {
        #define TYPE(T, gl_name) case gl_name : for (uint i = 0; i < size; ++i){memcpy(data, &T##s[uniform.offset + i], sizeOf); data = next_data(data, sizeOf, size);} break;
        TYPES
        #undef TYPE
      default:
        break;
      }
    }
  }
  uint buffer_size() const
  {
    return bufSize;
  }
};

class Material : IAsset
{
private:
  Shader shader;
  REFLECT(Material,
  (vector<Property>) (properties),
  (MaterialProperties) (materialProperties),
  (string) (shaderName))
public:
  Material() = default;
  Material(const vector<Property> & properties):
    properties(properties) { }
  Shader &get_shader();
  const Shader &get_shader() const;
  void set_property(const Property &property);
  void bind_to_shader() const;
  void unbind_to_shader() const;
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual void free() override;
  virtual bool edit() override;

  const MaterialProperties &get_properties() const;
  MaterialProperties &get_properties();
};
template<typename T>
class Asset;

Asset<Material> standart_material();
Asset<Material> standart_textured_material(Asset<Texture2D> texture);