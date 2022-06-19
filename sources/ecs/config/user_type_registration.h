#pragma once
#include "data_block/data_block.h"
#include "ecs/component_editor.h"
#include "data_block/data_block_serialization.h"
#include "user_type_info.h"



template<typename T>
bool template_component_edition(void *ptr, bool view_only)
{
  if constexpr (requires(T &v) { edit_component(v, "", false); })
  {
    return edit_component(*((T*)ptr), nameOf<T>::value.data(), view_only);
  }
  else
  {
    return false;
  }
}

template<typename T>
size_t template_serializer(std::ostream& os, const void *ptr)
{
  return write(os, *((T*)ptr));
}

template<typename T>
size_t template_deserializer(std::istream& is, void *ptr)
{
  return read(is, *((T*)ptr));
}
template<typename T>
void template_blk_constructor(const DataBlock &blk, void *ptr)
{
  if constexpr (std::is_constructible_v<T, const DataBlock &>)
  {
    new (ptr) T(blk);
  }
  else
  {
    new (ptr) T();
    read(blk, *((T*)ptr));
  }
}

template<typename T>
UserTypeInfo get_uset_type_info()
{
  return UserTypeInfo(
    template_component_edition<T>,
    template_serializer<T>,
    template_deserializer<T>,
    template_blk_constructor<T>);
}