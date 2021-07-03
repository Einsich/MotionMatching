#pragma once

namespace ecs
{
  enum class SystemTag
  {
    Game = 1 << 0,
    Editor = 1 << 1
  };
}