#include "texture.h"
static const int maxActiveTexturesCount = 10;
static bool activeTextures[maxActiveTexturesCount] = {false};

void Texture::bind(const Shader &shader, const char * textureName)
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
    log_error("Need more texture unit for %s", textureName);
    return;
  }
  
  activeTextures[activationUnit] = true;
  glActiveTexture(GL_TEXTURE0 + activationUnit);
  glBindTexture(textureType, textureObject);
  glUniform1i(glGetUniformLocation(shader.get_shader_program(), textureName), activationUnit);
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