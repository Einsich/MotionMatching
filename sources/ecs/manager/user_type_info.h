#pragma once
#include <iosfwd>


class DataBlock;

typedef bool (*ComponentEdition)(void*, bool);
typedef size_t (*Serializer)(std::ostream&, const void*);
typedef size_t (*Deserializer)(std::istream&, void*);
typedef void (*BlkRead)(const DataBlock &, void*);

struct UserTypeInfo
{
  const ComponentEdition componentEdition = nullptr;
  const Serializer serialiser = nullptr;
  const Deserializer deserialiser = nullptr;
  const BlkRead blkReader = nullptr;

  UserTypeInfo() = default;
  UserTypeInfo(
    ComponentEdition componentEdition,
    Serializer serialiser,
    Deserializer deserialiser,
    BlkRead blkReader):
  componentEdition(componentEdition),
  serialiser(serialiser),
  deserialiser(deserialiser),
  blkReader(blkReader)  
  {}
};