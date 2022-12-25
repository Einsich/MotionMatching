#pragma once
#include <ecs/singleton.h>
using uint = unsigned int;
struct EditorUI : ecs::Singleton
{
  uint windowFlags;
};