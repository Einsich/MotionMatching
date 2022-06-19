#pragma once
#include "manager/callable_description.h"
#include "ecs_event.h"
#include "manager/entity_id.h"

namespace ecs
{
  struct EventDescription final : CallableDescription
  {
    typedef  void (*EventHandler)(const Event&);
    typedef  void (*SingleEventHandler)(const Event&, ecs::EntityId);
    EventHandler broadcastEventHandler;
    SingleEventHandler unicastEventHandler;
    EventDescription(ecs::vector<EventDescription *> &handlers,
      const char *name, 
      ecs::vector<FunctionArgument> &&require_args,
      ecs::vector<FunctionArgument> &&require_not_args,
      ecs::vector<ecs::string> &&scenes,
      ecs::vector<ecs::string> &&before, ecs::vector<ecs::string> &&after,
      EventHandler broadcastEventHandler,
      SingleEventHandler unicastEventHandler,
      uint tags);
    void registration() override
    {
    }
  };

  typedef ecs::vector<std::pair<ecs::vector<EventDescription *>&, ecs::vector<EventDescription *>&>> AllHandlers;
  AllHandlers &get_all_event_handlers();
  template<typename T>
  ecs::vector<EventDescription *> &get_mutable_event_handlers();
  
  template<typename T>
  ecs::vector<EventDescription *> &get_event_handlers();
}