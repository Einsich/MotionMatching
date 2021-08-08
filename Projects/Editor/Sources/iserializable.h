#pragma once
#include <ostream>
#include <istream>
class ISerializable
{
public:
  virtual void serialize(std::ostream& os, bool bin, int depth) const = 0;
  virtual void deserialize(std::istream& is, bool bin) = 0;
};