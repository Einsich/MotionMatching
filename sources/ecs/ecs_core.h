#pragma once
#include <vector>
#include <queue>
#include <functional>
#include "manager/system_description.h"
namespace ecs
{
  template<typename E>
  struct EventDescription;
  template<typename E>
  struct SingleEventDescription;
  struct EntityContainer;
  struct Core
  {
    std::unordered_map<uint, FullTypeDescription> types;
    std::vector<SystemDescription*> systems;
    std::vector<QueryDescription*> queries;
    std::vector<QueryDescription*> event_queries;
    EntityContainer *entityContainer;
    std::queue<std::function<void()>> events;
    std::queue<EntityId> toDestroy;
    uint currentSceneTags;
    Core();
    ~Core();
    template<typename E>
    std::vector<EventDescription<E>*> &events_handler()
    {
      static std::vector<EventDescription<E>*> handlers;
      return handlers;
    }
    template<typename E>
    std::vector<SingleEventDescription<E>*> &single_events_handler()
    {
      static std::vector<SingleEventDescription<E>*> handlers;
      return handlers;
    }
    void destroy_all_entities();
    void destroy_entities_from_destroy_queue(bool with_swap_last_element);
    void replace_entity_container(EntityContainer *container);
    static bool allow_system_execute(uint tags, uint require_tags);
  };
  Core &core();

  template<typename E>
  struct EventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&);
    EventHandler eventHandler;
    uint tags;
    EventDescription(const char *name, const std::vector<FunctionArgument> &args, EventHandler eventHandler, uint tags):
      QueryDescription(name, args, false), eventHandler(eventHandler), tags(tags)
    {
      core().events_handler<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
      //printf("EventDescription %s\n", name);
    }
  };
  template<typename E>
  struct SingleEventDescription : QueryDescription
  {
    typedef  void (*EventHandler)(const E&, ecs::QueryIterator&);
    EventHandler eventHandler;
    uint tags;
    SingleEventDescription(const char *name, const std::vector<FunctionArgument> &args, EventHandler eventHandler, uint tags):
      QueryDescription(name, args, false), eventHandler(eventHandler), tags(tags)
    {
      core().single_events_handler<E>().push_back(this);
      core().event_queries.push_back((QueryDescription*)this);
    }
  };


  template<typename T>
  T* get_component(const EntityId &entity, const char *name);

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
  bool check_entity_container();
  pair<EntityId, Archetype&> add_entity(const vector<string_hash> & type_hashes);
  template<typename ...Args>
  EntityId create_entity(const ComponentInitializer<Args> &...args)
  {
    if (check_entity_container())
      return EntityId();
    vector<string_hash> typeHashes = 
      {register_type_description<EntityId>("eid"), component_initializer_hash(args)... };
    auto [eid, archetype] = add_entity(typeHashes);
    vector<void*> data = archetype.get_entity_data(typeHashes);
    component_copy(data[0], eid);
    component_initializer_copy<1>(data, args...);
    send_event_immediate(eid, OnEntityCreated());
    return eid;
  }
  EntityId create_entity(const string &template_name);
  EntityId create_entity(const char *template_name);
  struct Template;
  EntityId create_entity(const Template *temp);
  EntityId find_entity(uint archetype, uint index);
  void destroy_entity(const EntityId &eid);



  template<typename E>
  void send_event(const E &event)
  {
    core().events.push([event](){
      for (EventDescription<E> *descr : core().events_handler<E>())
        if (Core::allow_system_execute(descr->tags, core().currentSceneTags))
          descr->eventHandler(event);
    });
  }
  template<typename E>
  void send_event_immediate(const E &event)
  {
    for (EventDescription<E> *descr : core().events_handler<E>())
      if (Core::allow_system_execute(descr->tags, core().currentSceneTags))
        descr->eventHandler(event);
  }


  bool get_iterator(const QueryDescription &descr, const EntityId &eid, QueryIterator &iterator);
  bool get_iterator(const SingleQueryDescription &descr, const EntityId &eid, QueryIterator &iterator);


  template<typename E>
  void send_event(const EntityId &eid, const E &event)
  {
    if (eid)
    {
      core().events.push([eid, event](){
        for (SingleEventDescription<E> *descr : core().single_events_handler<E>())
        {
          if (Core::allow_system_execute(descr->tags, core().currentSceneTags))
          {
            QueryIterator iterator;
            if (get_iterator(*((QueryDescription*)descr), eid, iterator))
            {
              descr->eventHandler(event, iterator);
            }
          }
        } 
      });
    }
  }
  template<typename E>
  void send_event_immediate(const EntityId &eid, const E &event)
  {
    if (eid)
    {
      for (SingleEventDescription<E> *descr : core().single_events_handler<E>())
      {
        if (Core::allow_system_execute(descr->tags, core().currentSceneTags))
        {
          QueryIterator iterator;
          if (get_iterator(*((QueryDescription*)descr), eid, iterator))
          {
            descr->eventHandler(event, iterator);
          }
        }
      } 
    }
  }


  template<std::size_t... Is, std::size_t N>
  std::array<char*, N> copy_data_pointer_impl(uint binIndex,
    const std::array<vector<void*>*, N> &data_vectors, std::index_sequence<Is...>)
  {
    return std::array<char*, N> {(data_vectors[Is] ? ((char*)(*data_vectors[Is])[0]) : nullptr)...};
  }
  
  template<typename ...Args, std::size_t N, std::size_t... Is>
  void update_data_pointer_impl(std::array<char *, N> &pointers, std::index_sequence<Is...>)
  {
    std::array<char*, N> dummy{(pointers[Is] = pointers[Is] ? pointers[Is] + sizeof(std::remove_pointer_t<std::remove_cvref_t<Args>>) : pointers[Is])...};
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
  void perform_query_impl(const std::array<char *, N> &pointers, Callable function, std::index_sequence<Is...>)
  {
    function(get_smart_component<Is, Args>((std::remove_pointer_t<std::remove_cvref_t<Args>>*)pointers[Is])...);
  }

  template<typename ...Args, typename Callable>
  void perform_query(const QueryDescription &query, Callable function)
  {
    constexpr uint N = sizeof...(Args);
    constexpr auto indexes = std::make_index_sequence<N>();

    if constexpr (!std::conjunction_v<is_singleton<std::remove_pointer_t<std::remove_cvref_t<Args>>>...>)
    {
      for (uint archetypeIdx = 0, archetypeN = query.archetypes.size(); archetypeIdx < archetypeN; ++archetypeIdx)
      {
        const auto &cachedArchetype = query.archetypes[archetypeIdx];
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
  void perform_system(const QueryDescription &query, void(*function)(Args...))
  {
    perform_query<Args...>(query, function);
  }
  template<typename E, typename Event, typename ...Args>
  void perform_event(const E &event, const QueryDescription &query, void(*function)(Event, Args...))
  {
    perform_query<Args...>(query, [&](Args...args){function(event, args...);});
  }

  void system_statistic();
  void destroy_scene();
}
