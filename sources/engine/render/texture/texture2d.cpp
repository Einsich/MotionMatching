#include "texture2d.h"
#include "stb_image.h"
#include "resources/resource_registration.h"
#include "imgui.h"
#include "ecs/component_editor.h"
#include <parallel/thread_pool.h>

Texture2D::Texture2D()
{
  generateMips = false;
  textureType = TextureType::Texture2DType;
  colorFormat = TextureColorFormat::RGB;
  textureFormat = TextureFormat::UnsignedByte;
  wrapping = TextureWrappFormat::Repeat;
  pixelFormat = TexturePixelFormat::Linear;
  textureName = "";
  glGenTextures(1, &textureObject);
}
Texture2D::Texture2D(std::string texture_path_from_textures_folder,
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
    if (stbiData)
    {
      create_from_pointer(stbiData, textureHeight, textureWidth, sizeof(stbiData[0]));
      stbi_image_free(stbiData);
    }
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
  void Texture2D::load(const std::filesystem::path &path, bool reload, AssetStatus &status)
  {
    if ((!reload && status == AssetStatus::NotLoaded) || reload)
    {
      add_job([this, path]()
      {
        std::filesystem::path tmp = path;
        tmp.replace_extension("");
        load_from_path(tmp);//without .meta
        add_main_thread_job([this]()
        {
          if (stbiData)
          {
            create_from_pointer(stbiData, textureHeight, textureWidth, sizeof(stbiData[0]));
            stbi_image_free(stbiData);
          }
          //debug_log("async loaded texture2d %s", textureName.c_str());
        });
      });
      //status = AssetStatus::Loading;
      status = AssetStatus::Loaded;//workaround to avoid memory corruption
    }
  }
  bool is_power_of_2(int x) 
  {
    return x > 0 && !(x & (x - 1));
  }
  

  void Texture2D::load_from_path(const std::filesystem::path &path)
  {
    textureName = path.stem().string();
    int ch;
    stbi_set_flip_vertically_on_load(true);
    stbiData = stbi_load(path.string().c_str(), &textureWidth, &textureHeight, &ch, 0);

    switch (ch)
    {
      case 1: colorFormat = TextureColorFormat::R; break;
      case 2: colorFormat = TextureColorFormat::RG; break;
      case 3: colorFormat = TextureColorFormat::RGB; break;
      case 4: colorFormat = TextureColorFormat::RGBA; break;
    
      default:
        debug_error("Unsoported format for texture %s!", textureName.c_str());
        break;
    }
    textureFormat = TextureFormat::UnsignedByte;
		if (!stbiData)
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
    //if (!powerOfTwo && wrapping != TextureWrappFormat::ClampToEdge)
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