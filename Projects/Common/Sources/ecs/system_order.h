#pragma once

namespace ecs
{
  namespace SystemOrder
  {
    constexpr int
    SPECIAL = -2000,
    NO_ORDER = -1000,
    INPUT_DEPEND = 0,
    LOGIC = 1000,
    RENDER = 2000,
    UI = 3000,
    UIMENU = 4000;
  }
}