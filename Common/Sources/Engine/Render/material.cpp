#include "material.h"
#include <algorithm>
#include "Engine/Resources/resources.h"
#include "Engine/Resources/editor.h"
#include "ecs/component_editor.h"
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
void Material::load(const filesystem::path &, bool )
{

}
void Material::free()
{

}
bool Property::edit_property(Property& property)
{
  bool edited = false;
  constexpr int BUFN = 255;
  char buf[BUFN];
  snprintf(buf, BUFN, "%s", property.name.c_str());
  if (ImGui::InputText("##PropertyName", buf, BUFN))
    property.name.assign(buf), edited = true;
  if (property.name.empty())
    ImGui::TextColored(ImVec4(1,0,0,1), "Enter name");
  constexpr int propertyCount = 5;
  const char *names[propertyCount] = {"float" , "float2", "float3", "float4", "texture2D"};
  int curType = property.vecType - 1;
  
  if (ImGui::ListBox("##PropertyType", &curType, names, propertyCount, propertyCount))
    property.vecType = curType + 1, edited = true;
  switch (property.vecType)
  {
  case 1: edited |= ImGui::InputFloat("##Float", &property.property.x, 0.01, 10, 2); break;
  case 2: edited |= ImGui::InputFloat2("##Float2", &property.property.x, 2); break;
  case 3: edited |= ImGui::InputFloat3("##Float3", &property.property.x, 2); break;
  case 4: edited |= ImGui::InputFloat4("##Float4", &property.property.x, 2); break;
  case 5: edited |= edit_component(property.texture, "", false); break;
  default:
    break;
  }
  return edited;
}
bool Material::edit()
{
  std::function<bool(Property&)> f = Property::edit_property;
  return edit_vector(properties, "properties", f);
}
ResourceRegister<Material> materialRegister;

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
MaterialPtr standart_textured_material(Asset<Texture2D> texture)
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