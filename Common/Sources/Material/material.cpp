#include "material.h"
#include <algorithm>
void Property::bind_to_shader(const Shader &shader) const
{
  switch (vecType)
  {
    case 1: shader.set_float(name.c_str(), property.x); break;
    case 2: shader.set_vec2(name.c_str(), property); break;
    case 3: shader.set_vec3(name.c_str(), property); break;
    case 4: shader.set_vec4(name.c_str(), property); break;    
    case 5: texture->bind(shader, name.c_str()); break;    
    default:debug_error("Can't set %s to shader", name.c_str());break;
  }
}
void Property::unbind_to_shader(const Shader &shader) const
{
  switch (vecType)
  {
    case 1: shader.set_float(name.c_str(), 0); break;
    case 2: shader.set_vec2(name.c_str(), vec2()); break;
    case 3: shader.set_vec3(name.c_str(), vec3()); break;
    case 4: shader.set_vec4(name.c_str(), vec4()); break;    
    case 5: texture->unbind(); break;    
    default:debug_error("Can't unbind %s from shader", name.c_str());break;
  }
}
bool Property::operator== (const Property & other) const
{
  return name == other.name;
}
void Material::bind_to_shader(const Shader& shader) const
{
  for (const Property & property : properties)
    property.bind_to_shader(shader);
}
void Material::unbind_to_shader(const Shader &shader) const
{
  for (const Property & property : properties)
    property.unbind_to_shader(shader);
}

void Material::set_property(const Property &property)
{
  auto it = find(properties.begin(), properties.end(), property);
  if (it == properties.end())
  {
    properties.push_back(property);
  } 
  else
  {
    *it = property;
  }  
}
MaterialPtr make_material(const vector<Property> & properties)
{
  return make_shared<Material>(properties);
}
MaterialPtr standart_material()
{
  return make_material({
    Property("Ambient", vec3(0.2f, 0.2f, 0.2f)),
    Property("Diffuse", vec3(0.8f, 0.8f, 0.8f)),
    Property("Specular", vec3(0.9f, 0.9f, 0.9f)),
    Property("Shininess", 5.5f)
    });
}
MaterialPtr standart_textured_material(TexturePtr texture)
{
  return make_material({
    Property("Ambient", vec3(0.2f, 0.2f, 0.2f)),
    Property("Diffuse", vec3(0.8f, 0.8f, 0.8f)),
    Property("Specular", vec3(0.9f, 0.9f, 0.9f)),
    Property("Shininess", 5.5f),
    Property("uvScale", vec2(1.0f, 1.0f)),
    Property("uvOffset", vec2(0.0f, 0.0f)),
    Property("mainTex", texture)
    });
}