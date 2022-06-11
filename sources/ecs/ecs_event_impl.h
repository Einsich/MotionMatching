#pragma once
#include "manager/callable_description.h"
#include <vector>
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
    EventDescription(std::vector<EventDescription *> &handlers,
      const char *name, 
      std::vector<FunctionArgument> &&require_args,
      std::vector<FunctionArgument> &&require_not_args,
      std::vector<std::string> &&scenes,
      std::vector<std::string> &&before, std::vector<std::string> &&after,
      EventHandler broadcastEventHandler,
      SingleEventHandler unicastEventHandler,
      uint tags);
    void registration() override
    {
    }
  };

  typedef std::vector<std::pair<std::vector<EventDescription *>&, std::vector<EventDescription *>&>> AllHandlers;
  AllHandlers &get_all_event_handlers();
  template<typename T>
  std::vector<EventDescription *> &get_mutable_event_handlers();
  
  template<typename T>
  std::vector<EventDescription *> &get_event_handlers();
}