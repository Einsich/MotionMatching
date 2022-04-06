#include "heightmap.h"
#include <render/texture/stb_image.h>
#include <application/application.h>
#include <common.h>

float HeightMap::get_height(int x, int y) const
{
  size_t i = x + y * w;
  if (0 <= i && i < h * w)
    return heightMap[i];
  else
  {
    debug_error("(");
  }
  
  return 0;
}

float HeightMap::get_height(vec2 world_pos) const
{
  vec2 localPos = (world_pos - worldOffset) * worldScale;
  return get_height(localPos.x, localPos.y);
}

void HeightMap::load_heightmap(const std::string &path, float height_scale)
{
  int w, h, ch;
  stbi_set_flip_vertically_on_load(true);
  auto image = stbi_load(path.c_str(), &w, &h, &ch, 0);
  if (!image)
  {
    debug_error("invalid path %s", path.c_str());
    return;
  }
  this->h = h;
  this->w = w;
  auto imPtr = image;

  heightMap.resize(h*w);

  for (int i = 0, n = w * h; i < n; i++, imPtr += ch)
  {
    heightMap[i] = imPtr[0] / 255.f * height_scale;
  }
  stbi_image_free(image);
}