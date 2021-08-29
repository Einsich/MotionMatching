#include "material.h"
#include <algorithm>
#include "Engine/Resources/resources.h"
#include "Engine/Resources/editor.h"
#include "ecs/component_editor.h"
#include "global_uniform.h"

void Material::bind_textures_to_shader() const
{
  for (const SamplerUniform &sampler : shader.get_samplers())
  {
    uint textureObject = 0;
    uint textureType = 0;
    switch (sampler.type)
    {
      #define SAMPLER(T, gl_type) case gl_type: \
        textureObject = gl_type##s[sampler.arrayOffset]->get_texture_object(); \
        textureType = gl_type##s[sampler.arrayOffset]->get_texture_type(); break;
      SAMPLERS
      #undef SAMPLER
    }
    
    glActiveTexture(GL_TEXTURE0 + sampler.binding);
    glBindTexture(textureType, textureObject);
    glUniform1i(sampler.shaderLocation, sampler.binding);
  }
}

Shader &Material::get_shader()
{
  return shader;
}
const Shader &Material::get_shader() const
{
  return shader;
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

int Material::get_texture_index(const char *name, int gl_type) const
{
  auto it = uniformMap.find(name);
  if (it != uniformMap.end())
  {
    int i =  it->second;
    const vector<SamplerUniform> &samplers = shader.get_samplers();
    if ((uint)gl_type != samplers[i].type)
    {
      debug_error("bad formats for sampler %s %d in shader %d try set", name, samplers[i].type, gl_type);
      return -1;
    }
    return samplers[i].arrayOffset;
  } 
  debug_error("there is no texture %s", name);
  return -1;
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

BufferField Material::get_buffer_field(const char *name) const
{
  auto it = uniformMap.find(name);
  if (it != uniformMap.end())
  {
    int i = it->second;
    const StorageBuffer *instanceData = shader.get_instance_data();
    if (instanceData)
    {
      return instanceData->fields[i];
    }
    debug_error("there is field %s, but didn't instanceData", name);
    return BufferField();
  } 
  return BufferField();
}

void Material::load(const filesystem::path &, bool reload)
{
  if (!reload)
  {
    shader = ::get_shader(shaderName), debug_log("set shader %s to material", shaderName.c_str());
    uniformMap.clear();
    if (!shader)
      return;
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
    
    const vector<SamplerUniform> &samplers = shader.get_samplers();
    for (uint i = 0; i < samplers.size(); ++i)
    {
      const SamplerUniform &sampler = samplers[i];
      uniformMap[sampler.name] = i;
      switch (sampler.type)
      {
        #define SAMPLER(T, gl_type) case gl_type: if (sampler.arrayOffset + 1 > (int)gl_type##s.size()) \
          gl_type##s.resize(sampler.arrayOffset + 1); break;
        SAMPLERS
        #undef SAMPLER
      }
    }
    #define TYPE(T, _) for (const auto &p : T##savable) set_property(p.first.c_str(), p.second);
    #define SAMPLER(T, smp) for (const auto &p : smp##savable) set_texture(p.first.c_str(), p.second);
    TYPES
    SAMPLERS
    #undef TYPE
    #undef SAMPLER
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
      if (field.name.substr(0, 9) == "material.")
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
  
  #define SAMPLER(T, smp) smp##savable.clear();
  SAMPLERS
  #undef SAMPLER
  const vector<SamplerUniform> &samplers = shader.get_samplers();
  for (uint i = 0; i < samplers.size(); ++i)
  {
    const SamplerUniform &sampler = samplers[i];
    switch (sampler.type)
    {
      #define SAMPLER(T, smp) case smp: smp##savable.emplace_back(sampler.name, smp##s[sampler.arrayOffset]); break;
      SAMPLERS
      #undef SAMPLER
    }
  }
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
  {
    if (shader.get_shader_program() != tempShader.get_shader_program())
    {
      shader = tempShader;
      load("", false);
    }
  }
  
  const vector<SamplerUniform> &samplers = shader.get_samplers();
  for (uint i = 0; i < samplers.size(); ++i)
  {
    const SamplerUniform &sampler = samplers[i];
    switch (sampler.type)
    {
      #define SAMPLER(T, gl_type) case gl_type: edited |= edit_component(gl_type##s[sampler.arrayOffset], sampler.name.c_str(), false); break;
      SAMPLERS
      #undef SAMPLER
    }
  }

  const StorageBuffer *instanceData = shader.get_instance_data();
  if (instanceData)
  {
    for (const BufferField &field : instanceData->fields)
    {
      switch (field.type)
      {
        #define TYPE(T, gl_type) case gl_type: if (field.size == 1) edited |= edit_component(T##s[field.vectorOffset], field.name.c_str(), false); break;
        TYPES
        #undef TYPE
      }
    }
  }
  return edited;
}
ResourceRegister<Material> materialRegister;

