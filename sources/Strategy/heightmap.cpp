#include "heightmap.h"
#include <render/texture/stb_image.h>
#include <application/application.h>
#include <common.h>

float HeightMap::get_height(size_t x, size_t y) const
{
  if (0 <= x && x < w && 0 <= y && y < h)
    return heightMap[x + y * w];
  else
  {
    debug_error("(");
  }
  
  return 0;
}

bool HeightMap::get_height(size_t x, size_t y, float &height) const
{
  if (0 <= x && x < w && 0 <= y && y < h)
  {
    height = heightMap[x + y * w];
    return true;
  }  
  return false;
}

float HeightMap::get_height(vec2 world_pos) const
{
  vec2 localPos = (world_pos - worldOffset) * worldScale;
  return get_height(localPos.x, localPos.y);
}

bool HeightMap::get_height(vec3 world_pos, float &height) const
{
  vec2 localPos = (vec2(world_pos.x, world_pos.z) - worldOffset) * worldScale;
  return get_height(localPos.x, localPos.y, height);
}

void HeightMap::load_heightmap(const std::string &path, float height_scale, int water_level)
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
  waterLevel = water_level / 255.f;
  auto imPtr = image;

  heightMap.resize(h*w);

  float heightScale = 1.f / 255.f * height_scale;
  for (int i = 0, n = w * h; i < n; i++, imPtr += ch)
  {
    
    heightMap[i] = glm::max((int)imPtr[0], water_level) * heightScale;
  }
  stbi_image_free(image);
}

static void ray_trace_iterations(const HeightMap &height_map, vec3 &hitPos, vec3 n, float &height, float eps)
{
  constexpr int MAX_ITERATIONS = 100;

  for (int i = 0; i < MAX_ITERATIONS; i++)
  {
    float t = hitPos.y - height;
    hitPos += n * t;
    if (glm::abs(t) < eps)
      break;
    float h;
    if (height_map.get_height(hitPos, h))
      height = h;
    else
      break;
  }
}
bool HeightMap::ray_trace(const vec3 &p, const vec3 &n, vec3 &hitPos, float eps) const
{
  if (n.y >= 0)
  return false;
  constexpr float maxHeight = 1;
  float t = (p.y - maxHeight) / -n.y;
  hitPos = p + n * t;

  float height;
  if (get_height(hitPos, height))
  {
    ray_trace_iterations(*this, hitPos, n, height, eps);
    return true;
  }
  else
  {
    t = p.y / -n.y;
    hitPos = p + n * t;
    if (get_height(hitPos, height))
    {
      ray_trace_iterations(*this, hitPos, n, height, eps);
      return true;
    }
  }
  return false;
}