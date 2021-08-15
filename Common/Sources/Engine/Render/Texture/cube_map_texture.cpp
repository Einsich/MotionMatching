#include "cube_map_texture.h"
#include "stb_image.h"
#include "Engine/Resources/resources.h"
#include "ecs/component_editor.h"
CubeMap::CubeMap()
{
  textureName = "empty cubemap";
  textureType = TextureType::CubeMapType;
  colorFormat = TextureColorFormat::RGB;
  textureFormat = TextureFormat::UnsignedByte;
  pixelFormat = TexturePixelFormat::Linear;
  wrapping = TextureWrappFormat::Repeat;
  glGenTextures(1, &textureObject);
}
CubeMap::CubeMap(string cubemap_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixel_format)
{
  textureName = cubemap_path_from_textures_folder;
  textureType = TextureType::CubeMapType;
  colorFormat = color_format;
  textureFormat = texture_format;
  pixelFormat = pixel_format;  
  wrapping = TextureWrappFormat::Repeat;
  glGenTextures(1, &textureObject);
  load("", false);
}

void CubeMap::load(const filesystem::path &, bool reload)
{

  stbi_uc * images[6];
  int size = -1;
  string path = project_resources_path(textureName);
  for (int i = 0; i < 6; i++)
  {
    string fullpath = path + "/" + to_string(i) + ".jpg";
    int w, h, ch;
    stbi_set_flip_vertically_on_load(false);
    images[i] = stbi_load(fullpath.c_str(), &w, &h, &ch, 0);
    if (!images[i])
    {
      debug_error("Can't load face[%d] %s for cubemap!", i, fullpath.c_str());
      continue;
    }
    if (h != w)
    {
      debug_error("Cubmap face[%d] %s must have similar width and height (%d != %d) !", i, textureName.c_str(), w, h);
      return;
    }
    if (size < 0)
    {
      size = h;
    } else
    {
      if (size != h)
      {
        debug_error("Cubmap face[%d] %s must have similar sizes (%d != %d) !", i, textureName.c_str(), size, h);
        return;
      }
    }    
  }

  textureWidth = textureHeight = size;
  glBindTexture(textureType, textureObject);
  for (int i = 0; i < 6; i++)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, images[i]);
    stbi_image_free(images[i]);
  }
  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  GLenum minMagixelFormat = pixelFormat == TexturePixelFormat::Linear ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minMagixelFormat);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, minMagixelFormat);
  
  glBindTexture(textureType, 0); 
}
void CubeMap::free()
{

}
bool CubeMap::edit()
{
  bool edited = texture_edit();
  edited |= edit_component(textureName, "", false);
  return edited;
}

size_t CubeMap::serialize(std::ostream& os) const 
{
  return write(os, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight);
}
size_t CubeMap::deserialize(std::istream& is) 
{
  return read(is, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight);
}
ResourceRegister<CubeMap> cubeMapRegister;