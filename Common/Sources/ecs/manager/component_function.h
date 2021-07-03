#pragma once

namespace ecs
{
  typedef void *(*Constructor)(void*);
  typedef void *(*CopyConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
  typedef bool (*ComponentEdition)(void*, bool);
  typedef void (*Serializer)(void*, std::ostream&);
  typedef void (*Deserializer)(void*, std::istream&);
}