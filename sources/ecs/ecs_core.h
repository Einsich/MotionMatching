#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/system_description.h"
#include "template/blk_template.h"
namespace ecs
{
  template<typename E>
  struct EventDescription;

  struct EntityContainer;
  struct Core
  {
    std::unordered_map<uint, FullTypeDescription> types;
    std::vector<CallableDescription*> all_callable;
    std::vector<SystemDescription*> systems;
    std::vector<QueryDescription*> queries;
    std::vector<CallableDescription*> event_queries;
    EntityContainer *entityContainer;
    std::vector<std::function<void()>> events_cleaners;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    uint currentSceneTags;
    std::string sceneToLoad;
    bool reloadScene;
    Core();
    ~Core();
    
    struct EventsCleaner
    {
      EventsCleaner(std::function<void()> &&cleaner, Core &core)
      {
        core.events_cleaners.emplace_back(std::move(cleaner));
      }
    };
    template<typename E>
    std::vector<EventDescription<E>*> &events_handler()
    {
      static std::vector<EventDescription<E>*> handlers;
      static EventsCleaner cleaner([&](){handlers.clear();}, *this);
      return handlers;
    }
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue(bool with_swap_last_element);
    void update_systems_subscribes();
    void register_allowed_callable();
  };
  Core &core();

  template<typename E>
  struct EventDescription final : CallableDescription
  {
    typedef  void (*EventHandler)(const E&);
    typedef  void (*SingleEventHandler)(const E&, ecs::EntityId);
    EventHandler broadcastEventHandler;
    SingleEventHandler unicastEventHandler;
    EventDescription(const char *name, 
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      EventHandler broadcastEventHandler,
      SingleEventHandler unicastEventHandler,
      uint tags):
      CallableDescription(name, std::move(require_args), std::move(require_not_args), std::move(scenes), tags),
      broadcastEventHandler(broadcastEventHandler),
      unicastEventHandler(unicastEventHandler)
    {
      core().all_callable.push_back(this);
    }
    void registration() override
    {
      core().events_handler<E>().push_back(this);
      core().event_queries.push_back((CallableDescription*)this);
    }
  };

  template<typename T>
  struct ComponentInitializer
  {
    const char *name;
    const T & component;
  };
  template<typename T>
  string_hash component_initializer_hash(const ComponentInitializer<T> &component)
  {
    return register_type_description<T>(component.name);
  }
  template<typename T> 
  void component_copy(void *data, const T & component)
  {
    new (data) T(std::move(component));
  }
  template<int N, typename T, typename ...Args>
  void component_initializer_copy(vector<void*> &data, const ComponentInitializer<T> & arg, const Args &...args)
  {
    component_copy(data[N], arg.component);
    if constexpr (sizeof...(args) > 0)
      component_initializer_copy<N+1>(data, args...);
  }
  
  pair<EntityId, Archetype&> add_entity(const vector<string_hash> & type_hashes);
  template<typename ...Args>
  EntityId create_entity(const ComponentInitializer<Args> &...args)
  {
    vector<string_hash> typeHashes = 
      {register_type_description<EntityId>("eid"), component_initializer_hash(args)... };
    auto [eid, archetype] = add_entity(typeHashes);
    vector<void*> data = archetype.get_entity_data(typeHashes);
    component_copy(data[0], eid);
    component_initializer_copy<1>(data, args...);
    send_event_immediate(eid, OnEntityCreated());
    return eid;
  }
  struct Template;
  EntityId create_entity(const Template *temp, ComponentInitializerList &&list);
  EntityId create_entity(const Template *temp);
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);

  //destoy current scene and load new scene from path
  void create_scene(const string &path, bool reload = true);


  template<typename E>
  void send_event(const E &event)
  {
    core().events.push([event](){
      for (EventDescription<E> *descr : core().events_handler<E>())
        descr->broadcastEventHandler(event);
    });
  }
  template<typename E>
  void send_event_immediate(const E &event)
  {
    for (EventDescription<E> *descr : core().events_handler<E>())
      descr->broadcastEventHandler(event);
  }

  template<typename E>
  void send_event(EntityId eid, const E &event)
  {
    if (eid)
    {
      core().events.push([eid, event](){
        for (EventDescription<E> *descr : core().events_handler<E>())
          descr->unicastEventHandler(event, eid);
      });
    }
  }
  template<typename E>
  void send_event_immediate(EntityId eid, const E &event)
  {
    if (eid)
    {
      for (EventDescription<E> *descr : core().events_handler<E>())
        descr->unicastEventHandler(event, eid);
    }
  }


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
  void system_statistic();
}
