#include "texture2d_array.h"
#include "stb_image.h"
#include "imgui.h"
#include "component_editor.h"
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
void Texture2DArray::load(const filesystem::path &path, bool )
{
  vector<string> names = split(path.string(), '+');
  if (!names.empty())
    textureName = names[0];


  glBindTexture(textureType, textureObject);

  int layerCount = names.size() - 1;
  int mipLevelCount = 5;
  stbi_set_flip_vertically_on_load(true);
  for (uint i = 1; i < names.size(); i++)
  {
    int w, h, ch;
    string fullPath = root_path(names[i]);
    auto image = stbi_load(fullPath.c_str(), &w, &h, &ch, 0);
    if (i == 1)
    {
      glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, w, h, layerCount);
      textureWidth = w;
      textureHeight = h;
    }
    if (image)
    {
      if (w == textureWidth && h == textureHeight)
        glTexSubImage3D(textureType, 0, 0, 0, i-1, w, h, 1, colorFormat, textureFormat, image);
      else
        debug_error("bad size of %s [%dx%d] need [%dx%d] for %s texture2D array",
          names[i].c_str(), textureWidth, textureHeight, w, h, names[0].c_str());
      stbi_image_free(image);
    }
    else
      debug_error("can't load %s texture for %s texture2D array", names[i].c_str(), names[0].c_str());
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
