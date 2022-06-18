#pragma once

namespace ecs
{
  typedef void (*Constructor)(void*);
  typedef void (*CopyConstructor)(const void*, void*);
  typedef void (*MoveConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
}