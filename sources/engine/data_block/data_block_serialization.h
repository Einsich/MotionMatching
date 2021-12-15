#pragma once
#include "data_block.h"
#include "serialization/reflection.h"


template<typename T>
std::enable_if_t<!HasReflection<T>::value, void> read(const DataBlock &blk, T &t)
{
  if constexpr (std::is_constructible_v<T, const DataBlock &>)
  {
    t = T(blk);
  }
}
template<typename T>
void read(const DataBlock &blk, std::vector<T> &t)
{
  t.clear();
  
  if constexpr (DataBlock::AllowedTypes::hasType<T>())
  {
    constexpr size_t needType = DataBlock::AllowedTypes::getIndexOfType<T>();
    for (size_t i = 0, n = blk.propertiesCount(); i < n; i++)
      if (blk.getProperty(i).type == needType)
        t.push_back(blk.get<T>(blk.getProperty(i)));
  }
  else
  {
    constexpr const string_view &typeName = nameOf<T>::value;
    for (size_t i = 0, n = blk.blockCount(); i < n; i++)
      if (blk.getBlock(i)->type() == typeName)
      {
        T &arg = t.emplace_back();
        read(*blk.getBlock(i), arg);
      }
  }
}

template<typename T>
std::enable_if_t<HasReflection<T>::value, void> read(const DataBlock &blk, T &value)
{
  value.reflect([&](auto &arg, const char *name)
  { 
    using TT = std::remove_cvref_t<decltype(arg)>;
    if constexpr (DataBlock::AllowedTypes::hasType<TT>())
    {
      const auto *component = blk.get<TT>(name);
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


