
#include <ecs/ecs.h>
#include "ecs_module.h"

MAKE_EXTERNAL_TYPE_FACTORY(Function, das::Function)
MAKE_EXTERNAL_TYPE_FACTORY(ExprBlock, das::ExprBlock)
MAKE_EXTERNAL_TYPE_FACTORY(TypeDecl, das::TypeDecl)
MAKE_EXTERNAL_TYPE_FACTORY(Structure, das::Structure)

struct EventAnnotation : das::ManagedStructureAnnotation <ecs::Event> {
  EventAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation ("Event", ml, "ecs::Event") {
  }
};
struct RequestAnnotation : das::ManagedStructureAnnotation <ecs::Request> {
  RequestAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation ("Request", ml, "ecs::Request") {
  }
};

struct EntityIdAnnotation : das::ManagedValueAnnotation <ecs::EntityId> {
  EntityIdAnnotation() : ManagedValueAnnotation("EntityId", "ecs::EntityId") {
  }
};

class Module_ECS : public das::Module
{
public:
  Module_ECS() : Module("ecs_impl")
  {
    das::ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();
    addAnnotation(das::make_smart<EventAnnotation>(lib));
    addAnnotation(das::make_smart<RequestAnnotation>(lib));
    addAnnotation(das::make_smart<EntityIdAnnotation>());

    addExtern<DAS_BIND_FUN(::get_das_type_name)>(*this, lib, "get_das_type_name", das::SideEffects::modifyExternal, "::get_das_type_name");
    
    addExtern<DAS_BIND_FUN(::register_system)>(*this, lib, "register_system", das::SideEffects::modifyExternal, "::register_system");
    addExtern<DAS_BIND_FUN(::register_event)>(*this, lib, "register_event", das::SideEffects::modifyExternal, "::register_event");
    addExtern<DAS_BIND_FUN(::register_request)>(*this, lib, "register_request", das::SideEffects::modifyExternal, "::register_request");
    addExtern<DAS_BIND_FUN(::register_query)>(*this, lib, "register_query", das::SideEffects::modifyExternal, "::register_query");
    addExtern<DAS_BIND_FUN(::register_das_event)>(*this, lib, "register_das_event", das::SideEffects::modifyExternal, "::register_das_event");
    addExtern<DAS_BIND_FUN(::register_das_request)>(*this, lib, "register_das_request", das::SideEffects::modifyExternal, "::register_das_request");
    
    addExtern<DAS_BIND_FUN(::perform_query)>(*this, lib, "query", das::SideEffects::modifyExternal, "::perform_query");
    addExtern<DAS_BIND_FUN(::perform_eid_query)>(*this, lib, "query", das::SideEffects::modifyExternal, "::perform_eid_query");
    
    addExtern<DAS_BIND_FUN(::builtin_send_event)>(*this, lib, "builtin_send_event", das::SideEffects::modifyExternal, "::builtin_send_event");
    addExtern<DAS_BIND_FUN(::builtin_send_event_immediate)>(*this, lib, "builtin_send_event_immediate", das::SideEffects::modifyExternal, "::builtin_send_event_immediate");
    addExtern<DAS_BIND_FUN(::builtin_send_eid_event)>(*this, lib, "builtin_send_eid_event", das::SideEffects::modifyExternal, "::builtin_send_eid_event");
    addExtern<DAS_BIND_FUN(::builtin_send_eid_event_immediate)>(*this, lib, "builtin_send_eid_event_immediate", das::SideEffects::modifyExternal, "::builtin_send_eid_event_immediate");
    addExtern<DAS_BIND_FUN(::builtin_send_request)>(*this, lib, "builtin_send_request", das::SideEffects::modifyExternal, "::builtin_send_request");

    addExtern<DAS_BIND_FUN(::get_event_sizeof)>(*this, lib, "get_event_sizeof", das::SideEffects::accessExternal, "::get_event_sizeof");
    addExtern<DAS_BIND_FUN(::get_request_sizeof)>(*this, lib, "get_request_sizeof", das::SideEffects::accessExternal, "::get_request_sizeof");
    
    addExtern<DAS_BIND_FUN(ecs::event_name_to_index)>(*this, lib, "get_event_idx", das::SideEffects::modifyExternal, "ecs::event_name_to_index");
    addExtern<DAS_BIND_FUN(ecs::request_name_to_index)>(*this, lib, "get_request_idx", das::SideEffects::modifyExternal, "ecs::request_name_to_index");

    verifyAotReady();
  }
  virtual das::ModuleAotType aotRequire(das::TextWriter &tw) const override
  {
    // specifying which include files are required for this module
    // tw << "#include \"tutorial02aot.h\"\n";
    // specifying AOT type, in this case direct cpp mode (and not hybrid mode)
    return das::ModuleAotType::cpp;
  }
};

REGISTER_MODULE(Module_ECS);