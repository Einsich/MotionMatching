#include "texture2d_array.h"
#include "stb_image.h"
#include "imgui.h"
#include "component_editor.h"
#include <parallel/thread_pool.h>

Texture2DArray::Texture2DArray()
{
  generateMips = true;
  textureType = TextureType::Texture2DArrayType;
  colorFormat = TextureColorFormat::RGBA;
  textureFormat = TextureFormat::UnsignedByte;
  wrapping = TextureWrappFormat::Repeat;
  pixelFormat = TexturePixelFormat::Linear;
  textureName = "";
  glGenTextures(1, &textureObject);
}

static vector<string> split(const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim))
    if (item.size() > 0)
      result.emplace_back(item);
  return result;
}
void Texture2DArray::load(const filesystem::path &path, bool , AssetStatus &status)
{

  if (status == AssetStatus::NotLoaded)
  {
    add_job([this, &status, path]()
    {
      vector<string> names = split(path.string(), '+');
      if (!names.empty())
        textureName = names[0];
      vector<unsigned char*> images;
      bool haveErrors = false;
      for (uint i = 1; i < names.size(); i++)
      {
        int w, h, ch;
        string fullPath = root_path(names[i]);
        auto image = stbi_load(fullPath.c_str(), &w, &h, &ch, 0);
        if (i == 1)
        {
          textureWidth = w;
          textureHeight = h;
        }
        if (image)
        {
          images.push_back(image);
          if (!(w == textureWidth && h == textureHeight))
          {
            haveErrors = true;
            debug_error("bad size of %s [%dx%d] need [%dx%d] for %s texture2D array",
              names[i].c_str(), textureWidth, textureHeight, w, h, names[0].c_str());
            break;
          }
        }
        else
          debug_error("can't load %s texture for %s texture2D array", names[i].c_str(), names[0].c_str());
      }
      if (haveErrors)
        for (auto image : images)
          stbi_image_free(image);
      else
      add_main_thread_job([this, &status, images]()
      {
        int layerCount = images.size();
        int mipLevelCount = 5;
        glBindTexture(textureType, textureObject);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, textureWidth, textureHeight, layerCount);
        for (uint i = 0; i < images.size(); i++)
        {
          glTexSubImage3D(textureType, 0, 0, 0, i, textureWidth, textureHeight, 1, colorFormat, textureFormat, images[i]);
          if (images[i])
            stbi_image_free(images[i]);
        }

        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, TextureWrappFormat::Repeat);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, TextureWrappFormat::Repeat);
        
        if (generateMips)
        {
          glGenerateMipmap(textureType);
          GLenum mipmapMinPixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
          GLenum mipmapMagPixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
          
          glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, mipmapMinPixelFormat);
          glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, mipmapMagPixelFormat);
        }
        else
        {
          GLenum minMagixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
          glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minMagixelFormat);
          glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, minMagixelFormat);
        }
        glBindTexture(textureType, 0);
        status = AssetStatus::Loaded;
        //debug_log("async loaded cubemap %s", textureName.c_str());
      });
    });
    status = AssetStatus::Loading;
  }
}


bool Texture2DArray::edit()
{
  bool edited = texture_edit();

  if (ImGui::Checkbox("Generate mip maps", &generateMips))
    edited |= true;
  return edited;
}

size_t Texture2DArray::serialize(std::ostream& os) const 
{
  return write(os, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight, textureDepth, generateMips);
}
size_t Texture2DArray::deserialize(std::istream& is) 
{
  return read(is, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight, textureDepth, generateMips);
}
