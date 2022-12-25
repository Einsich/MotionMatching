#pragma once
#include <vector>
#include <string>
#include <3dmath.h>
#include <ecs/singleton.h>

struct HeightMap : ecs::Singleton
{

  std::vector<float> heightMap;
  size_t w, h;
  float waterLevel;
  vec2 worldOffset, worldScale;
  bool get_height(size_t x, size_t y, float &height) const;
  bool get_height(vec2 world_pos, float &height) const;
  bool is_land(vec2 world_pos, float &height) const;
  bool get_height(vec3 world_pos, float &height) const;
  void load_heightmap(const std::string &path, float height_scale, int water_level);

  bool ray_trace(const vec3 &p, const vec3 &n, vec3 &hitPos, float eps = 0.05f) const;
  bool world_to_pixel(const vec3 &world, int &x, int &y) const;
  bool world_to_pixel(const vec3 &world, ivec2 &pixel) const;

};