#pragma once
#include <iosfwd>


class DataBlock;

typedef bool (*ComponentEdition)(void*, bool);
typedef size_t (*Serializer)(std::ostream&, const void*);
typedef size_t (*Deserializer)(std::istream&, void*);

struct UserFunctions
{
  const ComponentEdition componentEdition = nullptr;
  const Serializer serialiser = nullptr;
  const Deserializer deserialiser = nullptr;

  UserFunctions() = default;
  UserFunctions(
    ComponentEdition componentEdition,
    Serializer serialiser,
    Deserializer deserialiser):
  componentEdition(componentEdition),
  serialiser(serialiser),
  deserialiser(deserialiser)
  {}
};