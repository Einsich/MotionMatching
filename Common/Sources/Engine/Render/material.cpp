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

pair<int, int> Material::get_uniform_index(const char *name, int gl_type) const
{
  auto it = uniformMap.find(name);
  if (it != uniformMap.end())
  {
    int i =  it->second;
    const StorageBuffer *instanceData = shader.get_instance_data();
    if (instanceData)
    {
      if (gl_type != instanceData->fields[i].type)
      {
        debug_error("bad formats for %s %d in shader %d try set", name, instanceData->fields[i].type, gl_type);
        return {-1, -1};
      }
      return {instanceData->fields[i].vectorOffset, instanceData->fields[i].size};
    }
    debug_error("there is field %s, but didn't instanceData", name);
    return{-1, -1};
  } 
  debug_error("there is no field %s", name);
  return {-1, -1};
}

uint Material::buffer_size() const
{
  const StorageBuffer *instanceData = shader.get_instance_data();
  return instanceData ? instanceData->size : 0;
}
void Material::set_data_to_buf(char *data) const
{
  const StorageBuffer *instanceData = shader.get_instance_data();
  if (instanceData)
  {
    for (const BufferField &field : instanceData->fields)
    {
      switch (field.type)
      {
        #define TYPE(T, gl_name) case gl_name : for (int i = 0; i < field.size; ++i)\
        {memcpy(data + field.offset + i * field.stride, &T##s[field.vectorOffset + i], sizeof(T##s[0]));} break;
        TYPES
        #undef TYPE
      default:
        break;
      }
    }
  }
}
void Material::load(const filesystem::path &, bool reload)
{
  if (!reload)
  {
    shader = ::get_shader(shaderName), debug_log("set shader %s to material", shaderName.c_str());
      
//uniformMap
//read from T##savable
    uniformMap.clear();
    const StorageBuffer *instanceData = shader.get_instance_data();
    if (instanceData)
    {
      for (uint i = 0; i < instanceData->fields.size(); ++i)
      {
        const BufferField &field = instanceData->fields[i];
        uniformMap[field.name] = i;
        switch (field.type)
        {
          #define TYPE(T, gl_type) case gl_type: if (field.vectorOffset + field.size > (int)T##s.size()) \
            T##s.resize(field.vectorOffset + field.size); break;
          TYPES
          #undef TYPE
        }
      }
    }
    #define TYPE(T, _) for (const auto &p : T##savable) set_property(p.first.c_str(), p.second);
    TYPES
    #undef TYPE
  }
}
void Material::free()
{
  
  #define TYPE(T, _) T##savable.clear();
  TYPES
  #undef TYPE
  const StorageBuffer *instanceData = shader.get_instance_data();
  if (instanceData)
  {
    for (const BufferField &field : instanceData->fields)
    {
      if (field.name.substr(0, 10) == "material.")
      {
        switch(field.type)
        {
          #define TYPE(T, gl_type) case gl_type: T##savable.emplace_back(field.name, \
            vector<T>(T##s.begin() + field.vectorOffset, T##s.begin() + field.vectorOffset + field.size)); break;
          TYPES
          #undef TYPE
        }
      }
    }
  }
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
  const StorageBuffer *instanceData = shader.get_instance_data();
  if (instanceData)
  {
    for (const BufferField &field : instanceData->fields)
    {
      switch (field.type)
      {
        #define TYPE(T, gl_type) case gl_type: if (field.size == 1) edit_component(T##s[field.vectorOffset], field.name.c_str(), false); break;
        TYPES
      
      default:
        break;
      }
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
