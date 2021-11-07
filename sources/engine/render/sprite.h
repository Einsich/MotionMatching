#pragma once
#include "texture/texture2d.h"
#include "3dmath.h"

struct Sprite
{
  Texture2D *texture;
  vec2 offset, scale;
  bool flipX, flipY;
  Sprite(): texture(nullptr), offset(0), scale(1), flipX(false), flipY(false)
  {

  }
  Sprite(Texture2D *texture, vec2 offset, vec2 scale, bool flipX = false, bool flipY = false): 
  texture(texture), offset(offset), scale(scale), flipX(flipX), flipY(flipY)
  {
    
  }
  Sprite(Texture2D *texture, bool flipX = false, bool flipY = false): 
  texture(texture), offset(0), scale(1), flipX(flipX), flipY(flipY)
  {
    
  }
};