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
  return 0;
}

int register_das_request(const das::StructurePtr &st)
{
  printf("register_das_event %s\n", st->name.c_str());
  return 0;
}

int get_event_sizeof(int type_id)
{
  return -1;
}

int get_request_sizeof(int type_id)
{
  return -1;
}

void builtin_send_event(int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_event_immediate(int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_eid_event(ecs::EntityId eid, int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_eid_event_immediate(ecs::EntityId eid, int size_of, int eventId, const void *event) { assert(0); }
void builtin_send_request(int size_of, int eventId, const void *event) { assert(0); }

ecs::EntityId create_entity_with_init(ecs::prefab_id id, const InitBlock &block, das::Context *context, das::LineInfoArg *at) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_with_init_n(const char *prefab_name, const InitBlock &block, das::Context *context, das::LineInfoArg *at) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity(const char *prefab_name) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_n(ecs::prefab_id id) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_immediate_with_init(ecs::prefab_id id, const InitBlock &block, das::Context *context, das::LineInfoArg *at) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_immediate_with_init_n(const char *prefab_name, const InitBlock &block, das::Context *context, das::LineInfoArg *at) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_immediate(const char *prefab_name) { assert(0); return ecs::EntityId(); }
ecs::EntityId create_entity_immediate_n(ecs::prefab_id id) { assert(0); return ecs::EntityId(); }
ecs::prefab_id create_entity_prefab(const char *name, const InitBlock &block, das::Context *context, das::LineInfoArg *at) { assert(0); return ecs::invalidPrefabId; }
vec4f init_component(das::Context &context, das::SimNode_CallBase *call, vec4f *args) { assert(0); return v_zero(); }