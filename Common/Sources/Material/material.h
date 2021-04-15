#pragma once
#include "common.h"
#include "Shader/shader.h"
#include "Texture/textures.h"
class Property
{
private:
  vec4 property;
  TexturePtr texture;
  string name;
  int vecType;
public:

  Property(const string& name, TexturePtr property)
    :property(vec4()), texture(property), name(name) {vecType = 5;}
  Property(const string& name, vec4 property)
    :property(property), texture(nullptr), name(name) {vecType = 4;}
  Property(const string& name, vec3 property)
    :Property(name, vec4(property, 0)) {vecType = 3;}
  Property(const string& name, vec2 property)
    :Property(name, vec3(property, 0)) {vecType = 2;}
  Property(const string& name, float property)
    :Property(name, vec2(property, 0)) {vecType = 1;}
  void bind_to_shader(const Shader &shader) const;
  void unbind_to_shader(const Shader &shader) const;
  bool operator== (const Property & other) const;
};
class Material
{
private:
  vector<Property> properties;
public:
  Material(const vector<Property> & properties):
    properties(properties) { }
  void set_property(const Property &property);
  void bind_to_shader(const Shader& shader) const;
  void unbind_to_shader(const Shader &shader) const;
};
using MaterialPtr = shared_ptr<Material>;

MaterialPtr make_material(const vector<Property> & properties);
MaterialPtr standart_material();
MaterialPtr standart_textured_material(TexturePtr texture);