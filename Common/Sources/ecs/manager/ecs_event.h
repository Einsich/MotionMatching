#pragma once

namespace ecs
{
  struct Event
  {

  };

  typedef  void (*EventHandler)(const Event&);
}