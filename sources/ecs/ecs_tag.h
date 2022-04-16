#pragma once
namespace ecs
{
  struct Tag
  {

  };
  namespace tags
  {
    constexpr uint32_t 
    debug = 1u << 0,
    all = 0xffffffff
    ;
  }
}