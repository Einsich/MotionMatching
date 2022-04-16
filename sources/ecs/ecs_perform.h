#pragma once
#include "ecs_core.h"

namespace ecs
{
  

  template<std::size_t... Is, std::size_t N>
  std::array<char*__restrict, N> copy_data_pointer_impl(uint binIndex,
    const std::array<vector<void*>*, N> &data_vectors, std::index_sequence<Is...>)
  {
    return std::array<char*__restrict, N> {(data_vectors[Is] ? (char*)((*data_vectors[Is])[binIndex]) : nullptr)...};
  }

  template<std::size_t N, std::size_t... Is>
  std::array<char*__restrict, N> data_pointer(uint binIndex, uint inBinIndex,
    const SystemCashedArchetype &archetype, std::index_sequence<Is...>)
  {
    return std::array<char*__restrict, N> {
      (archetype.containers[Is] ?
      ((char*)(archetype.containers[Is]->data[binIndex]) + archetype.containers[Is]->sizeOf * inBinIndex)
      : nullptr)...};
  }
  
  template<typename ...Args, std::size_t N, std::size_t... Is>
  void update_data_pointer_impl(std::array<char*__restrict, N> &pointers, std::index_sequence<Is...>)
  {
    std::array<char*__restrict, N> dummy{(pointers[Is] = pointers[Is] ? pointers[Is] + sizeof(std::remove_pointer_t<std::remove_cvref_t<Args>>) : pointers[Is])...};
    (void)dummy;
  }
  template<uint I, typename R, typename T>
  R get_smart_component(T arg)
  {

    using cvrefT = typename std::remove_cvref_t<R>;
    if constexpr (std::is_pointer<cvrefT>::value)
    {
      using smartT = typename std::remove_pointer_t<cvrefT>;
      if constexpr(is_singleton<smartT>::value)
        return &get_singleton<smartT>();
      else
        return arg ? (R)arg : nullptr;
    }
    else
    {
      if constexpr(is_singleton<cvrefT>::value)
        return get_singleton<cvrefT>();
      else
        return *arg;
    }
  }
  
  template<typename ...Args, typename Callable, std::size_t... Is, std::size_t N>
  void perform_query_impl(const std::array<char*__restrict, N> &pointers, Callable function, std::index_sequence<Is...>)
  {
    function(get_smart_component<Is, Args>((std::remove_pointer_t<std::remove_cvref_t<Args>>*)pointers[Is])...);
  }

  template<typename ...Args, typename Callable>
  void perform_query(const CallableDescription &descr, Callable function)
  {
    constexpr uint N = sizeof...(Args);
    constexpr auto indexes = std::make_index_sequence<N>();
    if constexpr (N == 0 || !std::conjunction_v<is_singleton<std::remove_pointer_t<std::remove_cvref_t<Args>>>...>)
    {
      if constexpr(N == 0)
      {
        if (descr.requireArgs.empty())
        {
          function();
          return;
        }
      }
      for (uint archetypeIdx = 0, archetypeN = descr.archetypes.size(); archetypeIdx < archetypeN; ++archetypeIdx)
      {
        const auto &cachedArchetype = descr.archetypes[archetypeIdx];
        if (cachedArchetype.archetype->count == 0)
          continue;
        
        std::array<vector<void*>*, N> dataVectors;
        for (uint i = 0; i < N; ++i)
          dataVectors[i] = cachedArchetype.containers[i] ? &cachedArchetype.containers[i]->data : nullptr;
        uint binN = (uint)cachedArchetype.archetype->count >> binPow;
        for (uint binIdx = 0; binIdx < binN; ++binIdx)
        {
          auto dataPointers = copy_data_pointer_impl(binIdx, dataVectors, indexes);
          for (uint inBinIdx = 0; inBinIdx < binSize; ++inBinIdx)
          {
            perform_query_impl<Args...>(dataPointers, function, indexes);
            update_data_pointer_impl<Args...>(dataPointers, indexes);
          }
        }
        uint lastBinSize = cachedArchetype.archetype->count - (binN << binPow);
        if (lastBinSize > 0)
        {
          auto dataPointers = copy_data_pointer_impl(binN, dataVectors, indexes);
          for (uint inBinIdx = 0; inBinIdx < lastBinSize; ++inBinIdx)
          {
            perform_query_impl<Args...>(dataPointers, function, indexes);
            update_data_pointer_impl<Args...>(dataPointers, indexes);
          }
        }
      }
    }
    else
    {
      function(get_singleton<std::remove_pointer_t<std::remove_cvref_t<Args>>>()...);
    }
  }
  
  template<typename ...Args>
  void perform_system(const CallableDescription &descr, void(*function)(Args...))
  {
    perform_query<Args...>(descr, function);
  }
  template<typename E, typename Event, typename ...Args>
  void perform_event(const E &event, const CallableDescription &descr, void(*function)(Event, Args...))
  {
    perform_query<Args...>(descr, [&](Args...args){function(event, args...);});
  }


  template<typename ...Args, typename Callable>
  void perform_query(const CallableDescription &descr, EntityId eid, Callable function)
  {
    constexpr uint N = sizeof...(Args);
    constexpr auto indexes = std::make_index_sequence<N>();
    if (eid)
    {
      uint index = eid.array_index();
      int archetypeInd = eid.archetype_index();
      auto it = std::find_if(descr.archetypes.begin(), descr.archetypes.end(),
        [archetypeInd](const SystemCashedArchetype &cashed_archetype){return cashed_archetype.archetype->index == archetypeInd;});
      if (it != descr.archetypes.end())
      {
        uint binIdx = index >> binPow;
        uint inBinIdx = index & binMask;
        auto dataPointers = data_pointer<N>(binIdx, inBinIdx, *it, indexes);
        perform_query_impl<Args...>(dataPointers, function, indexes);
      }
    }
  }

  template<typename E, typename Event, typename ...Args>
  void perform_event(const E &event, const CallableDescription &descr, EntityId eid, void(*function)(Event, Args...))
  {
    perform_query<Args...>(descr, eid, [&](Args...args){function(event, args...);});
  }
} // namespace ecs
