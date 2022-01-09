#include "texture.h"
#include "imgui.h"

static const int maxActiveTexturesCount = 10;
static bool activeTextures[maxActiveTexturesCount] = {false};

void Texture::bind(const Shader &shader, const char * textureName) const
{
  int offset = 0;
  for (; offset < maxActiveTexturesCount; offset++)
  {
    if (!activeTextures[offset])
    {
      activationUnit = offset;
      break;
    }
  }
  if (offset == maxActiveTexturesCount)
  {
    debug_error("Need more texture unit for %s", textureName);
    return;
  }
  
  activeTextures[activationUnit] = true;
  glActiveTexture(GL_TEXTURE0 + activationUnit);
  glBindTexture(textureType, textureObject);
  glUniform1i(glGetUniformLocation(shader.get_shader_program(), textureName), activationUnit);
}
void Texture::bind(const Shader &shader, const char * textureName, int slot) const
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(textureType, textureObject);
  glUniform1i(glGetUniformLocation(shader.get_shader_program(), textureName), slot);
}
const string &Texture::get_name() const
{
  return textureName;
}
void Texture::unbind() const
{
  activeTextures[activationUnit] = false;
  glBindTexture(textureType, 0);
}
void Texture::dispose()
{
  glDeleteTextures(1, &textureObject);
  textureName = "Bad texture";
}
int Texture::width() const
{
  return textureWidth;
}
int Texture::height() const
{
  return textureHeight;
}
int Texture::depth() const
{
  return textureDepth;
}
uint Texture::get_texture_object() const
{
  return textureObject;
}

uint Texture::get_texture_type() const
{
  return textureType;
}

GLenum Texture::get_internal_format(TextureColorFormat colorFormat, TextureFormat textureFormat)
{
 switch (colorFormat)
 {
  case R: switch (textureFormat)
  {
    case HalfFloat:     return GL_R16F; break;
    case Float:         return GL_R32F; break;
    default:break;
  }break;
  case RG: return RG; break;
  case RGB: return RGB; break;
  case RGBA: return RGBA; break;

  case RI: switch (textureFormat)
  {
    case UnsignedByte:  return GL_R8UI; break;
    case Byte:          return GL_R8I; break;
    case UnsignedShort: return GL_R16UI; break;
    case Short:         return GL_R16I; break;
    case UnsignedInt:   return GL_R32UI; break;
    case Int:           return GL_R32I; break;
    default:break;
  }break;
  default:return colorFormat; break;
 }
 return colorFormat; 
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
bool Texture::texture_edit()
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
  return edited;
}