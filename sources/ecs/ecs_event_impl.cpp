#include "ecs_event_impl.h"

namespace ecs
{
  EventDescription::EventDescription(
    std::vector<EventDescription *> &handlers,
    const char *name, 
    std::vector<FunctionArgument> &&require_args,
    std::vector<FunctionArgument> &&require_not_args,
    std::vector<std::string> &&scenes,
    std::vector<std::string> &&before, std::vector<std::string> &&after,
    EventHandler broadcastEventHandler,
    SingleEventHandler unicastEventHandler,
    uint tags):
    CallableDescription(name, std::move(require_args), std::move(require_not_args),
      std::move(scenes), std::move(before), std::move(after), tags),
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