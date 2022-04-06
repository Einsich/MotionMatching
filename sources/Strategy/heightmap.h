#pragma once
#include <vector>
#include <string>
#include <3dmath.h>
#include <singleton.h>

struct HeightMap : ecs::Singleton
{

  std::vector<float> heightMap;
  size_t w, h;
  vec2 worldOffset, worldScale;
  float get_height(int x, int y) const;
  float get_height(vec2 world_pos) const;
  void load_heightmap(const std::string &path, float height_scale);

};