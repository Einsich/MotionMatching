#pragma once
#include <daScript/daScript.h>
#include <ecs/ecs.h>

MAKE_TYPE_FACTORY(string, ecs::string)
MAKE_TYPE_FACTORY(Event, ecs::Event)
MAKE_TYPE_FACTORY(Request, ecs::Request)
MAKE_TYPE_FACTORY(EntityId, ecs::EntityId)
MAKE_TYPE_FACTORY(OnEntityCreated, ecs::OnEntityCreated)
MAKE_TYPE_FACTORY(OnEntityDestroyed, ecs::OnEntityDestroyed)
MAKE_TYPE_FACTORY(OnEntityTerminated, ecs::OnEntityTerminated)
MAKE_TYPE_FACTORY(OnSceneCreated, ecs::OnSceneCreated)
MAKE_TYPE_FACTORY(OnSceneTerminated, ecs::OnSceneTerminated)

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
    const das::FunctionPtr &system,
    uint64_t mangled_hash);

bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event,
    uint64_t mangled_hash);

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request,
    uint64_t mangled_hash);

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block);
  
void perform_eid_query(ecs::EntityId eid, const das::Block &block, das::Context *context, das::LineInfoArg *line);
void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line);

inline const char* get_das_type_name(const das::TypeDecl &type)
{

  switch (type.baseType)
  {
  case das::Type::tStructure: return type.structType->name.c_str(); break;
  case das::Type::tHandle: return type.annotation->name.c_str(); break;
  case das::Type::tInt: return "int"; break;
  case das::Type::tInt2: return "int2"; break;
  case das::Type::tInt3: return "int3"; break;
  case das::Type::tInt4: return "int4"; break;
  case das::Type::tFloat: return "float"; break;
  case das::Type::tFloat2: return "vec2"; break;
  case das::Type::tFloat3: return "vec3"; break;
  case das::Type::tFloat4: return "vec4"; break;
  case das::Type::tDouble: return "double"; break;
  case das::Type::tString: return "string"; break;
  case das::Type::tBool: return "bool"; break;
  
  default: return "unsopported type"; break;
  } 
}
int register_das_event(const das::StructurePtr &st);
int register_das_request(const das::StructurePtr &st);

inline int get_event_sizeof(int type_id)
{
  const auto &types = ecs::get_all_registered_events();
  return (uint32_t)type_id < types.size() ? types[type_id].sizeOf : 0;
}

inline int get_request_sizeof(int type_id)
{
  const auto &types = ecs::get_all_registered_requests();
  return (uint32_t)type_id < types.size() ? types[type_id].sizeOf : 0;
}
void builtin_send_event(int size_of, int eventId, const void *event);
void builtin_send_event_immediate(int size_of, int eventId, const void *event);
void builtin_send_eid_event(ecs::EntityId eid, int size_of, int eventId, const void *event);
void builtin_send_eid_event_immediate(ecs::EntityId eid, int size_of, int eventId, const void *event);
void builtin_send_request(int size_of, int eventId, const void *event);