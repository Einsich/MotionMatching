#pragma once

namespace ecs
{
  typedef void *(*Constructor)(void*);
  typedef void *(*CopyConstructor)(void*, void*);
  typedef void (*Destructor)(void*);
  typedef bool (*ComponentEdition)(void*, bool);
  typedef size_t (*Serializer)(std::ostream&, void*);
  typedef size_t (*Deserializer)(std::istream&, void*);
}