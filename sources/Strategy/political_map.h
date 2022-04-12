#pragma once

#include <vector>
#include <string>
#include <3dmath.h>
#include <singleton.h>

struct Country
{
  vec3 color;
  int id;
  std::string name;
};
struct Province
{
  vector<int> borderIndexes;
};

struct PoliticalMap : ecs::Singleton
{
  int w, h;
  vector<Country> countries;
  vector<uint> provincesIdx;
  //readonly
  vector<uvec2> provincesInfo;
  vector<Province> provinces;
  //key = a << 16 | b, a < b, value = {border index, swaped}
  map<uint, pair<int, bool>> borderIndexes;
  enum
  {
    MAX_PROVINCES = 1 << 10,
    PROVINCES_MASK = MAX_PROVINCES - 1
  };
};