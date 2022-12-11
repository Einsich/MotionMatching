#include "ecs_event_impl.h"

namespace ecs
{
  EventDescription::EventDescription(
    ecs::vector<EventDescription *> &handlers,
    const char *name,
    ecs::vector<FunctionArgument> &&require_args,
    ecs::vector<FunctionArgument> &&require_not_args,
    ecs::vector<ecs::string> &&before, ecs::vector<ecs::string> &&after,
    EventHandler broadcastEventHandler,
    SingleEventHandler unicastEventHandler,
    ecs::vector<ecs::string> &&tags):
    CallableDescription(name, std::move(require_args), std::move(require_not_args),
    std::move(before), std::move(after), std::move(tags)),
    broadcastEventHandler(broadcastEventHandler),
    unicastEventHandler(unicastEventHandler)
  {
    handlers.push_back(this);
  }

  AllHandlers&get_all_event_handlers()
  {
    static AllHandlers handlers;
    return handlers;
  }

}