#pragma once
#include "data_block.h"
#include "serialization/reflection.h"


template<typename T>
std::enable_if_t<!HasReflection<T>::value, void> read(const DataBlock &, T &)
{

}
template<typename T>
std::enable_if_t<HasReflection<T>::value, void> read(const DataBlock &blk, T &value)
{
  value.reflect([&](auto &arg, const char *name)
  { 
    if constexpr (DataBlock::AllowedTypes::hasType<T>())
    {
      const auto *component = blk.get<std::remove_cvref_t<decltype(arg)>>(name);
      if (component)
        arg = *component;
    }
    else
    {
      const DataBlock *nonTrivialComponent = blk.getBlock(name);
      if (nonTrivialComponent)
        read(*nonTrivialComponent, arg);
    }
  });
}


template<typename T>
void write(DataBlock &blk, const T &value)
{

}


