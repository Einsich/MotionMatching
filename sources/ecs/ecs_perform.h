#pragma once
#include "ecs_core.h"
#include "ecs_event_impl.h"
#include <parallel/thread_pool.h>

namespace ecs
{

  template<typename OutType, typename InType>
  OutType __forceinline get_smart_component2(InType arg, size_t i)
  {
    using cvrefT = typename std::remove_cvref_t<OutType>;
    if constexpr (std::is_pointer<cvrefT>::value)
    {
      using smartT = typename std::remove_pointer_t<cvrefT>;
      if constexpr(is_singleton<smartT>::value)
        return &get_singleton<smartT>();
      else
        return arg ? arg + i : nullptr;
    }
    else
    {
      if constexpr(is_singleton<cvrefT>::value)
        return get_singleton<cvrefT>();
      else
        return *(arg + i);
    }
  }
  template<typename ...InArgs, typename Callable, typename ...OutArgs>
  void __forceinline perform_loop2(Callable function, size_t n, OutArgs ...containers)
  {
    for (size_t i = 0; i < n; ++i)
    {
      function(get_smart_component2<InArgs>(containers, i)...);
    }
  }
  template<std::size_t N, typename ...Args, typename Callable, std::size_t... Is>
  void __forceinline perform_loop(const ecs::vector<ecs::ComponentContainer *> &containers, Callable function, size_t i, size_t n, std::index_sequence<Is...>)
  {
    perform_loop2<Args...>(function, n, (std::remove_pointer_t<std::remove_reference_t<Args>>*)(containers[Is] ? containers[Is]->data[i] : nullptr)...);
  }

  template<typename ...InArgs, typename Callable, typename ...OutArgs>
  void __forceinline perform_query2(Callable function, size_t j, OutArgs ...containers)
  {
    function(get_smart_component2<InArgs>(containers, j)...);
  }
  template<std::size_t N, typename ...Args, typename Callable, std::size_t... Is>
  void __forceinline perform_query(const ecs::vector<ecs::ComponentContainer *> &containers, Callable function, size_t i, size_t j, std::index_sequence<Is...>)
  {
    perform_query2<Args...>(function, j, (std::remove_pointer_t<std::remove_reference_t<Args>>*)(containers[Is] ? containers[Is]->data[i] : nullptr)...);
  }


  template<typename ...Args, typename Callable>
  void __forceinline perform_query(const CallableDescription &descr, Callable function)
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

        uint binN = (uint)cachedArchetype.archetype->count >> binPow;
        for (uint binIdx = 0; binIdx < binN; ++binIdx)
        {
          perform_loop<N, Args...>(cachedArchetype.containers, function, binIdx, binSize, indexes);
        }
        uint lastBinSize = cachedArchetype.archetype->count - (binN << binPow);
        if (lastBinSize > 0)
        {
          perform_loop<N, Args...>(cachedArchetype.containers, function, binN, lastBinSize, indexes);
        }
      }
    }
    else
    {
      function(get_singleton<std::remove_pointer_t<std::remove_cvref_t<Args>>>()...);
    }
  }

  template<typename ...Args>
  void __forceinline perform_system(const CallableDescription &descr, void(*function)(Args...))
  {
    perform_query<Args...>(descr, function);
  }

  template<typename ...Args>
  void __forceinline perform_job_system(const CallableDescription &descr, void(*function)(Args...))
  {
    add_job([&descr, function](){ perform_query<Args...>(descr, function); });
  }

  template<typename E, typename Event, typename ...Args>
  void __forceinline perform_event(const E &event, const CallableDescription &descr, void(*function)(Event, Args...))
  {
    perform_query<Args...>(descr, [&](Args...args){function(event, args...);});
  }


  template<typename ...Args, typename Callable>
  void __forceinline perform_query(const CallableDescription &descr, EntityId eid, Callable function)
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
        perform_query<N, Args...>(it->containers, function, binIdx, inBinIdx, indexes);
      }
    }
  }

  template<typename E, typename Event, typename ...Args>
  void perform_event(const E &event, const CallableDescription &descr, EntityId eid, void(*function)(Event, Args...))
  {
    perform_query<Args...>(descr, eid, [&](Args...args){function(event, args...);});
  }
} // namespace ecs
