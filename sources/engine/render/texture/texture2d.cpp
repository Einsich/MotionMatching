#include "texture2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "resources/resource_registration.h"
#include "imgui.h"
#include "component_editor.h"
Texture2D::Texture2D()
{
  generateMips = false;
  textureType = TextureType::Texture2DType;
  colorFormat = TextureColorFormat::RGB;
  textureFormat = TextureFormat::Byte;
  wrapping = TextureWrappFormat::Repeat;
  pixelFormat = TexturePixelFormat::Linear;
  textureName = "";
  glGenTextures(1, &textureObject);
}
Texture2D::Texture2D(string texture_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixel_format,
  TextureWrappFormat wrapping_format, 
  bool generate_mips)
  {
    generateMips = generate_mips;
    textureType = TextureType::Texture2DType;
    colorFormat = color_format;
    textureFormat = texture_format;
    wrapping = wrapping_format;
    pixelFormat = pixel_format;
    
    glGenTextures(1, &textureObject);
    load_from_path(texture_path_from_textures_folder);
  }
  Texture2D::Texture2D(int w, int h,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixel_format,
  TextureWrappFormat wrapping_format, 
  bool generate_mips)
  {
    generateMips = generate_mips;
    textureType = TextureType::Texture2DType;
    colorFormat = color_format;
    textureFormat = texture_format;
    wrapping = wrapping_format;
    pixelFormat = pixel_format;
    
    glGenTextures(1, &textureObject);
    create_from_pointer(nullptr, h, w, 0);

  }
  void Texture2D::load(const filesystem::path &path, bool )
  {
    filesystem::path tmp = path;
    tmp.replace_extension("");
    load_from_path(tmp);//without .meta
  }
  bool is_power_of_2(int x) 
  {
    return x > 0 && !(x & (x - 1));
  }
  

  void Texture2D::load_from_path(const filesystem::path &path)
  {
    textureName = path.stem().string();
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    auto image = stbi_load(path.string().c_str(), &w, &h, &ch, 0);
    if (image)
    {
      create_from_pointer(image, h, w, sizeof(image[0]));
    }
		else
		{
			debug_error("Can't load texture %s!", textureName.c_str());
		}
  }

  void Texture2D::create_from_pointer(const unsigned char *image, int h, int w, size_t size)
  {
    powerOfTwo = is_power_of_2(w) && is_power_of_2(h);

    glBindTexture(textureType, textureObject);
    textureWidth = w;
    textureHeight = h;

    bool rightSizeOf = true;
    switch(textureFormat)
    {
      case TextureFormat::Byte :
      case TextureFormat::UnsignedByte:
      rightSizeOf = size == 1;
      break;
      case TextureFormat::Short :
      case TextureFormat::UnsignedShort :
      case TextureFormat::HalfFloat :
      rightSizeOf = size == 2;
      break;  
      case TextureFormat::Int :
      case TextureFormat::UnsignedInt :
      case TextureFormat::Float :
      rightSizeOf = size == 4;
      break;  
    }
    GLenum format = get_internal_format(colorFormat, textureFormat);
    if (rightSizeOf || size == 0)
        glTexImage2D(textureType, 0, format, textureWidth, textureHeight, 0, colorFormat, textureFormat, image);
    else
      debug_error("Wrong sizeof for texture format");
    if (!powerOfTwo && wrapping != TextureWrappFormat::ClampToEdge)
    {
      glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapping);
      glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapping);
    }
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

  void Texture2D::update_sub_region(int level, int x_offset, int y_offset, int width, int height, const void *data)
  {    
    glTextureSubImage2D(textureObject, level, x_offset, y_offset, width, height, colorFormat, textureFormat, data);
  }
bool Texture2D::edit()
{
  bool edited = texture_edit();

  if (ImGui::Checkbox("Generate mip maps", &generateMips))
    edited |= true;
  return edited;
}

size_t Texture2D::serialize(std::ostream& os) const 
{
  return write(os, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight, generateMips);
}
size_t Texture2D::deserialize(std::istream& is) 
{
  return read(is, textureName, textureType, colorFormat, textureFormat, wrapping, 
              pixelFormat, textureWidth, textureHeight, generateMips);
}
ResourceRegister<Texture2D> texture2DRegister({".jpg", ".png", ".bmp"});