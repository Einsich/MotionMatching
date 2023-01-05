#pragma once
#include <daScript/daScript.h>
#include <ecs/ecs.h>

MAKE_TYPE_FACTORY(Event, ecs::Event)
MAKE_TYPE_FACTORY(Request, ecs::Request)
MAKE_TYPE_FACTORY(EntityId, ecs::EntityId)

namespace das {
  template <>
  struct cast<ecs::EntityId> {
    static_assert(sizeof(ecs::EntityId) <= sizeof(vec4f));

    static __forceinline ecs::EntityId to(vec4f x)  { ecs::EntityId y; memcpy(&y, &x, sizeof(ecs::EntityId)); return y; }
    static __forceinline vec4f from(ecs::EntityId x){ vec4f y;         memcpy(&y, &x, sizeof(ecs::EntityId)); return y; }
  };
}
void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system);

bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event);

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request);

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block);
  
void perform_eid_query(ecs::EntityId eid, const das::Block &block, das::Context *context, das::LineInfoArg *line);
void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line);
const char* get_das_type_name(const das::TypeDecl &type);