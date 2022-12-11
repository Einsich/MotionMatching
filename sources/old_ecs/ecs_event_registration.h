#pragma once
#include "ecs_event_impl.h"

namespace ecs
{
  template<typename T>
  struct EventRegister
  {
    static_assert(std::is_base_of<Event, T>::value);
    EventRegister()
    {
      get_all_event_handlers().emplace_back(get_mutable_event_handlers<T>(), get_event_handlers<T>());
      auto unicast = unicast_event<T>;
      auto broadcast = broadcast_event<T>;
      (void)(unicast);//only need to instantiate this functions
      (void)(broadcast);
    }
  };
}

#define ECS_EVENT_REGISTER(VAR_NAME, EVENT)\
namespace ecs {\
  template<> ecs::vector<EventDescription *> &get_mutable_event_handlers<EVENT>()\
  {\
    static ecs::vector<EventDescription *> handlers;\
    return handlers;\
  }\
  template<> ecs::vector<EventDescription *> &get_event_handlers<EVENT>()\
  {\
    static ecs::vector<EventDescription *> handlers;\
    return handlers;\
  }\
  template<> void unicast_event<EVENT>(const Event &event, EntityId eid)\
  {\
    for (const EventDescription *descr : get_event_handlers<EVENT>())\
      descr->unicastEventHandler(event, eid);\
  }\
  template<> void broadcast_event<EVENT>(const Event &event)\
  {\
    for (const EventDescription *descr : get_event_handlers<EVENT>())\
      descr->broadcastEventHandler(event);\
  }\
  static EventRegister<EVENT> VAR_NAME##Register;\
}