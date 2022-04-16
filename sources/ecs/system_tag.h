#pragma once
namespace ecs
{
  namespace SystemTag
  {
    constexpr uint32_t 
    Game = 1 << 0u,
    Editor = 1 << 1u,
    GameEditor = Game | Editor,
    Debug = 1 << 2u,
    All = 0xffffffff;
  }
}