#pragma once
#include "ecs/singleton.h"

struct EditorUI : ecs::Singleton
{
  uint windowFlags;
};