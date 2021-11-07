#pragma once
#include <ostream>
#include <istream>
class ISerializable
{
public:
  virtual size_t serialize(std::ostream& os) const = 0;
  virtual size_t deserialize(std::istream& is) = 0;
};