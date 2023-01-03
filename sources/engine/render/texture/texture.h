#pragma once
#include "../shader/shader.h"
#include "resources/asset.h"
enum TextureType
{
  Texture2DType = GL_TEXTURE_2D,
  Texture3DType = GL_TEXTURE_3D,
  CubeMapType = GL_TEXTURE_CUBE_MAP,
  Texture2DArrayType = GL_TEXTURE_2D_ARRAY
};
enum TextureColorFormat
{
  R = GL_RED,
  RG = GL_RG,
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  Depth = GL_DEPTH_COMPONENT,
  DepthStencil = GL_DEPTH_STENCIL,
  RI = GL_RED_INTEGER
};
enum TextureFormat
{
  UnsignedByte =  GL_UNSIGNED_BYTE,
  Byte =  GL_BYTE,
  UnsignedShort = GL_UNSIGNED_SHORT,
  Short = GL_SHORT,
  UnsignedInt = GL_UNSIGNED_INT,
  Int = GL_INT,
  HalfFloat = GL_HALF_FLOAT,
  Float = GL_FLOAT
};
enum TextureWrappFormat
{
  Repeat = GL_REPEAT,
  ClampToEdge = GL_CLAMP_TO_EDGE,
  ClampToBorder = GL_CLAMP_TO_BORDER
};
enum TexturePixelFormat
{
  Pixel,
  Linear
};
class Texture
{
  
protected:
  GLuint textureObject;
  TextureType textureType;
  TextureColorFormat colorFormat;
  TextureFormat textureFormat;
  TextureWrappFormat wrapping;
  TexturePixelFormat pixelFormat;
  int textureWidth, textureHeight, textureDepth;
  std::string textureName;
  bool powerOfTwo;
  GLenum get_internal_format(TextureColorFormat colorFormat, TextureFormat textureFormat);
  bool texture_edit();
private:
  mutable GLuint activationUnit;
public:
  Texture():textureObject(0), textureDepth(1){}
  void bind(const Shader &shader, const char * textureName) const;
  void bind(const Shader &shader, const char * textureName, int slot) const;
  const std::string &get_name() const;
  uint get_texture_object() const;
  uint get_texture_type() const;
  void unbind() const;
  void dispose();
  int width() const;
  int height() const;
  int depth() const;
  
};