#pragma once

namespace ecs
{
  namespace SystemOrder
  {
    constexpr int
    NO_ORDER = -1000,
    LOGIC = 1000,
    RENDER = 2000,
    UI = 3000,
    UIMENU = 4000;
  }
}