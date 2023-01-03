#pragma once

#include "texture.h"

class Texture3D : public Texture
{
public:
  Texture3D(){}
  Texture3D(int size_x, int size_y, int size_z,
  const std::string & name,
  TextureColorFormat color_format = TextureColorFormat::R, 
  TextureFormat texture_format = TextureFormat::Float, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat);
void execute_dispatch(int work_group_size = 8);
};
