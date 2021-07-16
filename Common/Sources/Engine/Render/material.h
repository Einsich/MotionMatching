#pragma once
#include "common.h"
#include "Shader/shader.h"
#include "Texture/textures.h"
#include "Engine/Resources/asset.h"
#include "Serialization/iserializable.h"
class Property : public ISerializable
{
private:
  string name;
  int vecType;
  vec4 property;
  Asset<Texture2D> texture;
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
class Material : IAsset
{
private:
  REFLECT(Material,
  (vector<Property>) (properties)
  )
public:
  Material() = default;
  Material(const vector<Property> & properties):
    properties(properties) { }
  void set_property(const Property &property);
  void bind_to_shader(const Shader& shader) const;
  void unbind_to_shader(const Shader &shader) const;
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual void free() override;
  virtual bool edit() override;
};
using MaterialPtr = shared_ptr<Material>;

MaterialPtr make_material(const vector<Property> & properties);
MaterialPtr standart_material();
MaterialPtr standart_textured_material(Asset<Texture2D> texture);