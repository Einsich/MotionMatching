#pragma once

namespace ecs
{
  typedef void *(*Constructor)(void*);
  typedef void *(*CopyConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
  typedef void (*ComponentEdition)(void*);
}