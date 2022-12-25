#pragma once
#include "user_type_info.h"

#include <magic_enum.hpp>
#include <eastl/array.h>
#include "data_block/data_block.h"
#include "component_editor.h"
#include "data_block/data_block_serialization.h"
#include <imgui.h>

template <typename T>
bool edit_enum(T &component, const char *, bool view_only)
{
  constexpr std::size_t count = magic_enum::enum_count<T>();
  constexpr auto values = magic_enum::enum_values<T>();
  constexpr auto names = magic_enum::enum_names<T>();

  eastl::array<const char *, count> tmpNames;
  int curItem = 0;
  for (uint i = 0; i < count; i++)
  {
    if (values[i] == component)
      curItem = i;
    tmpNames[i] = names[i].data();
  }

  bool edited = ImGui::ListBox("##enum", &curItem, tmpNames.data(), count);
  if (edited && !view_only)
  {
    component = values[curItem];
  }
  return edited;
}

template <typename T>
bool template_component_edition(void *ptr, bool view_only)
{
  if constexpr (requires(T & v) { edit_component(v, "", false); })
  {
    return edit_component(*((T *)ptr), nameOf<T>::value.data(), view_only);
  }
  if constexpr (std::is_enum_v<T>)
  {
    return edit_enum(*((T *)ptr), nameOf<T>::value.data(), view_only);
  }
  else
  {
    return false;
  }
}

template <typename T>
size_t template_serializer(std::ostream &os, const void *ptr)
{
  return write(os, *((T *)ptr));
}

template <typename T>
size_t template_deserializer(std::istream &is, void *ptr)
{
  return read(is, *((T *)ptr));
}
template <typename T>
void template_blk_constructor(void *ptr, const ecs::ComponentPrefab &prefab, bool /* is_rvalue */)
{
  if constexpr ( DataBlock::AllowedTypes::hasType<T>())
  {
    if (prefab.is<T>())
      new (ptr) T(prefab.get<T>());
    else
      new (ptr) T();
  }
  else
  {
    if (prefab.is<const DataBlock*>())
    {
      const DataBlock *blk = prefab.get<const DataBlock*>();
      if constexpr (std::is_constructible_v<T, const DataBlock &>)
      {
        new (ptr) T(*blk);
      }
      else
      {
        new (ptr) T();
        read(*blk, *((T *)ptr));
      }
    }
    else
    {
      if (prefab.is<T>())
        new (ptr) T(prefab.get<T>());
      else
        new (ptr) T();
    }
  }
}

template <typename T>
UserFunctions get_user_functions()
{
  return UserFunctions(
      template_component_edition<T>,
      template_serializer<T>,
      template_deserializer<T>);
}