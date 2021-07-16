#include "texture2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Engine/Resources/resources.h"
#include "Engine/imgui/imgui.h"
#include "ecs/component_editor.h"
Texture2D::Texture2D()
{
  generateMips = false;
  textureType = TextureType::Texture2DType;
  colorFormat = TextureColorFormat::RGB;
  textureFormat = TextureFormat::Byte;
  wrapping = TextureWrappFormat::Repeat;
  pixelFormat = TexturePixelFormat::Linear;
  textureName = "empty texture";
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
      powerOfTwo = is_power_of_2(w) && is_power_of_2(h);
  
      glBindTexture(textureType, textureObject);
			textureWidth = w;
			textureHeight = h;

      size_t size = sizeof(image[0]);
      switch(textureFormat)
      {
        case TextureFormat::Byte :
        case TextureFormat::UnsignedByte:
        if (size == 1) 
			    glTexImage2D(textureType, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, image);
        else
          debug_error("Wrong sizeof for texture format");
        break;
        case TextureFormat::Short :
        case TextureFormat::UnsignedShort :
        case TextureFormat::HalfFloat :
        if (size == 2) 
			    glTexImage2D(textureType, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, image);
        else
          debug_error("Wrong sizeof for texture format");
        break;  
        case TextureFormat::Int :
        case TextureFormat::UnsignedInt :
        case TextureFormat::Float :
        if (size == 4) 
			    glTexImage2D(textureType, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, textureFormat, image);
        else
          debug_error("Wrong sizeof for texture format");
        break;  
      }
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
		else
		{
			debug_error("Can't load texture %s!",textureName.c_str());
			return;
		}
  }

void Texture2D::free()
{

}
template<int N, typename E>
bool list_box(const array<const char *, N> &str, const array<E, N> &e, const char* label, E &curE)
{
  static int curStr = -1;
  static bool buttonPressed = false;
  int curI = 0;
  bool edited = false;
  for (; curI < N && curE != e[curI]; curI++);

  if (ImGui::Button(str[curI]))
    buttonPressed = !buttonPressed;

  if (buttonPressed)
  {
    if (ImGui::ListBox(label, &curStr, str.data(), N, N))
    {
      edited = true;
      curE = e[curStr];
      curStr = -1;
      buttonPressed = false;
    }
  }
  return edited;
}
bool Texture2D::edit()
{
  bool edited = false;
  ImGui::Text("%s [%d x %d]", textureName.c_str(), textureWidth, textureHeight);

  edited |= list_box<6, TextureColorFormat>({"R", "RG", "RGB", "RGBA", "Depth", "DepthStensil"}, 
  {TextureColorFormat::R,TextureColorFormat::RG,TextureColorFormat::RGB,
    TextureColorFormat::RGBA,TextureColorFormat::Depth, TextureColorFormat::DepthStencil},
    "Color format", colorFormat);
  
  edited |= list_box<8, TextureFormat>({"Byte", "UnsignedByte", "Short", "UnsignedShort", "HalfFloat", "Int", "UnsignedInt", "Float"}, 
  {TextureFormat::Byte, TextureFormat::UnsignedByte, TextureFormat::Short, 
  TextureFormat::UnsignedShort, TextureFormat::HalfFloat, TextureFormat::Int, TextureFormat::UnsignedInt, TextureFormat::Float},
    "Texture format", textureFormat);

  edited |= list_box<3, TextureWrappFormat>({"Repeat", "ClampToBorder", "ClampToEdge"}, 
  {TextureWrappFormat::Repeat, TextureWrappFormat::ClampToBorder, TextureWrappFormat::ClampToEdge},
    "Wrapping", wrapping);

  edited |= list_box<2, TexturePixelFormat>({"Point", "Linear"}, 
  {TexturePixelFormat::Pixel, TexturePixelFormat::Linear},
    "Pixel format", pixelFormat);

  if (ImGui::Checkbox("Generate mip maps", &generateMips))
    edited |= true;
  return edited;
}

template<>
bool edit_component(Asset<Texture2D> &component, const char *name, bool view_only)
{
  ImGui::Text("Texture2D %s %s", component->get_name().c_str(), name);
  bool edited = false;
  if (!view_only)
  {
    static bool select = false;
    if (ImGui::Button("Select texture"))
      select = !select;
    if (select)
    {
      vector<const char *> names;
      const auto &resMap = Resources::instance().assets[nameOf<Texture2D>::value];
      for (const auto &asset : resMap.resources)
      {
        names.push_back(asset.first.c_str());
      }
      static int curTex = -1;
      if (ImGui::ListBox("", &curTex, names.data(), names.size(), 10))
      {
        select = false;
        auto it = resMap.resources.find(string(names[curTex]));
        if (it != resMap.resources.end())
        {
          component = *((Asset<Texture2D>*)&it->second);
          edited = true;
        }
      }
    }
  }
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
ResourceRegister<Texture2D> texture2DRegister({".jpg", ".png"});