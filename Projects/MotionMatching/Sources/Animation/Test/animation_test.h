#pragma once
#include "Engine/input.h"
#include "Serialization/serialization.h"
struct AnimationTest final : public ISerializable
{
  std::vector<KeyEventAnyActionKey> keyboardEvents;
  std::vector<MouseMoveEvent> mouseMoveEvents;
  std::string name;
  float totalTime;
  virtual size_t serialize(std::ostream& os) const override
  {
    size_t size = 0;
    size += write(os, keyboardEvents);
    size += write(os, mouseMoveEvents);
    size += write(os, name);
    size += write(os, totalTime);
    return size;
  }
  virtual size_t deserialize(std::istream& is) override
  {
    size_t size = 0;
    size += read(is, keyboardEvents);
    size += read(is, mouseMoveEvents);
    size += read(is, name);
    size += read(is, totalTime);
    return size;
  }
  
};