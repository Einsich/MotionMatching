#include "../das_modules/ecs_module.h"

void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system,
    uint64_t mangled_hash)
{
  printf("register_system %s\n", system->name.c_str());
}


bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event,
    uint64_t mangled_hash)
{
  printf("register_event %s\n", event->name.c_str());
  return true;
}

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request,
    uint64_t mangled_hash)
{
  printf("register_request %s\n", request->name.c_str());
  return true;
}

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block)
{
  printf("register_query %s:%d\n", block->at.fileInfo->name.c_str(), block->at.line);
}

void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line)
{
  assert(0);
}

void perform_eid_query(ecs::EntityId eid, const das::Block &block, das::Context *context, das::LineInfoArg *line)
{
  assert(0);
}

int register_das_event(const das::StructurePtr &st)
{
  printf("register_das_event %s\n", st->name.c_str());
}

int register_das_request(const das::StructurePtr &st)
{
  printf("register_das_event %s\n", st->name.c_str());
}
void builtin_send_event(int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_event_immediate(int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_eid_event(ecs::EntityId eid, int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_eid_event_immediate(ecs::EntityId eid, int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_request(int size_of, int eventId, const void *event) { assert(0); }