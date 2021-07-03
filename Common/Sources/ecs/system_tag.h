#pragma once

namespace ecs
{
  enum class SystemTag : uint32_t
  {
    Game = 1 << 0,
    Editor = 1 << 1
  };
  constexpr enum SystemTag operator|(const enum SystemTag selfValue, const enum SystemTag inValue )
  {
      return (enum SystemTag)(uint32_t(selfValue) | uint32_t(inValue));
  }
}