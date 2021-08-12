#include "material.h"
#include <algorithm>
#include "Engine/Resources/resources.h"
#include "Engine/Resources/editor.h"
#include "ecs/component_editor.h"
#include "global_uniform.h"
void Property::bind_to_shader(const Shader &shader) const
{
  switch (vecType)
  {
    case 1: shader.set_float(name.c_str(), property.x); break;
    case 2: shader.set_vec2(name.c_str(), property); break;
    case 3: shader.set_vec3(name.c_str(), property); break;
    case 4: shader.set_vec4(name.c_str(), property); break;    
    case 5: if (texture) texture->bind(shader, name.c_str()); break;    
    default:
    //debug_error("Can't set %s to shader", name.c_str());
    break;
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
    case 5: if (texture) texture->unbind(); break;    
    default:
    //debug_error("Can't unbind %s from shader", name.c_str());
    break;
  }
}
bool Property::operator== (const Property & other) const
{
  return name == other.name;
}
size_t Property::serialize(std::ostream& os) const
{
  size_t size = write(os, name, vecType);
  switch (vecType)
  {
    case 1: case 2: case 3: case 4: size += write(os, property); break;
    case 5: size += write(os, texture); break;    
    default:break;
  }
  return size;
}
size_t Property::deserialize(std::istream& is)
{
  size_t size = read(is, name, vecType);
  switch (vecType)
  {
    case 1: case 2: case 3: case 4: size += read(is, property); break;
    case 5: size += read(is, texture); break;    
    default:break;
  }
  return size;  
}
void Material::bind_to_shader() const
{
  for (const Property & property : properties)
    property.bind_to_shader(shader);
}
void Material::unbind_to_shader() const
{
  for (const Property & property : properties)
    property.unbind_to_shader(shader);
}

Shader &Material::get_shader()
{
  return shader;
}
const Shader &Material::get_shader() const
{
  return shader;
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
void Material::load(const filesystem::path &, bool reload)
{
  if (!reload)
  {
    shader = ::get_shader(shaderName), debug_log("set shader %s to material", shaderName.c_str());
    GLuint program = shader.get_shader_program();
    int count;
    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 128; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

    for (int i = 0; i < count; i++)
    {
      glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
      char instanceData[] = "InstanceData";
      char *subStr = strstr(name, instanceData);
      if (subStr)
      {
        subStr += sizeof(instanceData);
        materialProperties.add_uniform(size, type, subStr);
        debug_log("Uniform #%d Type: %u Size: %u Name: %s\n", i, type, size, subStr);
      }
    }
  }
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
bool select_string(const vector<const char *> &names, string &name, const char *label, bool &selecting)
{
  if (selecting)
  {
    int curItem = -1;
    if (ImGui::ListBox("", &curItem, names.data(), names.size(), 10))
    {
      name.assign(names[curItem]);
      selecting = false;
      return true;
    }
    ImGui::SameLine();
    if (ImGui::Button("back##selecting"))
    {
      selecting = false;
      return false;
    }
  }
  else
  {
    if (ImGui::Button(label))
    {
      selecting = true;
    }
  }
  return false;
}
bool Material::edit()
{
  static bool selecting = false;
  if (!selecting)
  {
    ImGui::Text("shader: \"%s\"", shaderName.c_str());
    ImGui::SameLine();
  }
  bool edited = select_string(get_shaders_names(), shaderName, "select", selecting);

  Shader tempShader = ::get_shader(shaderName, false);
  if (!tempShader)
  {
    if (selecting)
      ImGui::SameLine();
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Bad shader name");
  }
  else
    shader = tempShader;

  std::function<bool(Property&)> f = Property::edit_property;
  edited |= edit_vector(properties, "properties", f);
  for (const auto &uniformP : materialProperties.uniformMap)
  {
    const char * name = uniformP.first.c_str();
    uint offset = materialProperties.uniforms[uniformP.second].offset;
    uint size = materialProperties.uniforms[uniformP.second].size;
    switch (materialProperties.uniforms[uniformP.second].type)
    {
      #define TYPE(T, gl_type) case gl_type: if (size == 1) edit_component(materialProperties.T##s[offset], name, false); break;
      TYPES
    
    default:
      break;
    }
  }
  
  return edited;
}
ResourceRegister<Material> materialRegister;

Asset<Material> standart_material()
{
  return get_resource<Material>("standart").copy();
}
Asset<Material> standart_textured_material(Asset<Texture2D> texture)
{
  Asset<Material> material = get_resource<Material>("standart").copy();
  material->set_property(Property("mainTex", texture));
  material->set_property(Property("uvScale", vec2(1.f)));
  material->set_property(Property("uvOffset", vec2(0.f)));
  return material;
}
