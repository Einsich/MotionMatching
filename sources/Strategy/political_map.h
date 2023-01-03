#pragma once

#include <vector>
#include <eastl/vector.h>
#include <eastl/map.h>
#include <string>
#include <3dmath.h>
#include <ecs/singleton.h>

struct Country
{
  vec3 color;
  int id;
  std::string name;
};
struct Province
{
  eastl::vector<int> borderIndexes;
};

struct PoliticalMap : ecs::Singleton
{
  int w, h;
  eastl::vector<Country> countries;
  eastl::vector<uint> provincesIdx;
  //readonly
  eastl::vector<uvec2> provincesInfo;
  eastl::vector<Province> provinces;
  //key = a << 16 | b, a < b, value = {border index, swaped}
  eastl::map<uint, eastl::pair<int, bool>> borderIndexes;
  enum
  {
    MAX_PROVINCES = 1 << 10,
    PROVINCES_MASK = MAX_PROVINCES - 1
  };
};