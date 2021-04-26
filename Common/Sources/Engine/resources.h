#pragma once
#include "common.h"
#include <map>
#include "Render/Texture/texture.h"
#include "Render/mesh.h"


class Resources
{
  std::map<std::string, TexturePtr> textures;
  std::map<std::string, MeshPtr> meshes;
  static inline Resources *instance;
  static Resources &get_instance()
  {
    return (instance ? *instance : *(instance = new Resources()));
  }
public:
  static void add_texture(const std::string &name, TexturePtr texture)
  {
    auto it = get_instance().textures.find(name);
    if (it == get_instance().textures.end())
    {
      get_instance().textures[name] = texture;
    }
    else
    {
      debug_error("texture %s already was added", name.c_str());
    }
  }
  static void add_mesh(const std::string &name, MeshPtr mesh)
  {
    auto it = get_instance().meshes.find(name);
    if (it == get_instance().meshes.end())
    {
      get_instance().meshes[name] = mesh;
    }
    else
    {
      debug_error("mesh %s already was added", name.c_str());
    }
  }
  static TexturePtr get_texture(const std::string &name)
  {
    auto it = get_instance().textures.find(name);
    if (it != get_instance().textures.end())
    {
      return it->second;
    }
    else
    {
      debug_error("texture %s didn't added in resources", name.c_str());
    }
  }
  static MeshPtr get_mesh(const std::string &name)
  {
    auto it = get_instance().meshes.find(name);
    if (it != get_instance().meshes.end())
    {
      return it->second;
    }
    else
    {
      debug_error("mesh %s didn't added in resources", name.c_str());
    }
  }
  static void dispose_resources()
  {
    get_instance().~Resources();
  }
};