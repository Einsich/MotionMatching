#pragma once

namespace ecs
{
  enum class SystemOrder
  {
    SPECIAL = -200,
    NO_ORDER = -100,
    INPUT_DEPEND = 0,
    LOGIC = 100,
    EARLY_RENDER = 200,
    MIDDLE_RENDER = 300,
    LATE_RENDER = 400,
    UI = 500
  };
}
#define SYSTEM_ORDER(order)