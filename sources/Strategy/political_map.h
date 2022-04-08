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

struct PoliticalMap : ecs::Singleton
{
  vector<Country> countries;
  vector<uint> provincesIdx;
  //readonly
  vector<uvec2> provincesInfo;
};