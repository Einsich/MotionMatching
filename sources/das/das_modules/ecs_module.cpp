
#include <ecs/ecs.h>
#include "ecs_module.h"

MAKE_EXTERNAL_TYPE_FACTORY(Function, das::Function)
MAKE_EXTERNAL_TYPE_FACTORY(ExprBlock, das::ExprBlock)
MAKE_EXTERNAL_TYPE_FACTORY(TypeDecl, das::TypeDecl)
MAKE_EXTERNAL_TYPE_FACTORY(Structure, das::Structure)

#define VOID_STRUCTURE_ANNOTATION(ANNOTATION, NAME, CPP_TYPE)                                \
struct ANNOTATION : das::ManagedStructureAnnotation <CPP_TYPE> {                             \
  ANNOTATION(das::ModuleLibrary & ml) : ManagedStructureAnnotation (NAME, ml, #CPP_TYPE) {}  \
}
#define VOID_VALUE_ANNOTATION(ANNOTATION, NAME, CPP_TYPE)                                \
struct ANNOTATION : das::ManagedValueAnnotation <CPP_TYPE> {                             \
  ANNOTATION() : ManagedValueAnnotation (NAME, #CPP_TYPE) {}                             \
}

VOID_STRUCTURE_ANNOTATION(EventAnnotation, "Event", ecs::Event);
VOID_STRUCTURE_ANNOTATION(RequestAnnotation, "Request", ecs::Request);
VOID_VALUE_ANNOTATION(EntityIdAnnotation, "EntityId", ecs::EntityId);
VOID_STRUCTURE_ANNOTATION(ComponentInitializerAnnotation, "ComponentInitializer", ComponentInitializer);

VOID_STRUCTURE_ANNOTATION(OnEntityCreatedAnnotation, "OnEntityCreated", ecs::OnEntityCreated);
VOID_STRUCTURE_ANNOTATION(OnEntityDestroyedAnnotation, "OnEntityDestroyed", ecs::OnEntityDestroyed);
VOID_STRUCTURE_ANNOTATION(OnEntityTerminatedAnnotation, "OnEntityTerminated", ecs::OnEntityTerminated);
VOID_STRUCTURE_ANNOTATION(OnSceneCreatedAnnotation, "OnSceneCreated", ecs::OnSceneCreated);
VOID_STRUCTURE_ANNOTATION(OnSceneTerminatedAnnotation, "OnSceneTerminated", ecs::OnSceneTerminated);


struct StringAnnotation : das::ManagedStructureAnnotation <ecs::string> {
  StringAnnotation(das::ModuleLibrary & ml) : ManagedStructureAnnotation ("ecs_string", ml, "ecs::string")
  {
    addProperty<DAS_BIND_MANAGED_PROP(size)>("length", "size");
    addProperty<DAS_BIND_MANAGED_PROP(c_str)>("c_str", "c_str");
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
    addAnnotation(das::make_smart<OnEntityCreatedAnnotation>(lib));
    addAnnotation(das::make_smart<OnEntityDestroyedAnnotation>(lib));
    addAnnotation(das::make_smart<OnEntityTerminatedAnnotation>(lib));
    addAnnotation(das::make_smart<OnSceneCreatedAnnotation>(lib));
    addAnnotation(das::make_smart<OnSceneTerminatedAnnotation>(lib));
    addAnnotation(das::make_smart<StringAnnotation>(lib));
    addAnnotation(das::make_smart<ComponentInitializerAnnotation>(lib));

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

    addExtern<DAS_BIND_FUN(ecs::event_name_to_index)>(*this, lib, "get_event_idx", das::SideEffects::accessExternal, "ecs::event_name_to_index");
    addExtern<DAS_BIND_FUN(ecs::request_name_to_index)>(*this, lib, "get_request_idx", das::SideEffects::accessExternal, "ecs::request_name_to_index");
    addExtern<DAS_BIND_FUN(ecs::type_name_to_index)>(*this, lib, "get_component_type_idx", das::SideEffects::accessExternal, "ecs::type_name_to_index");


    addExtern<ecs::uint(*)(const char*), ecs::get_prefab_id>(*this, lib, "get_prefab_id", das::SideEffects::accessExternal, "ecs::get_prefab_id");
    addExtern<DAS_BIND_FUN(::create_entity_with_init)>(*this, lib, "create_entity", das::SideEffects::modifyExternal, "::create_entity_with_init");
    addExtern<DAS_BIND_FUN(::create_entity_with_init_n)>(*this, lib, "create_entity", das::SideEffects::modifyExternal, "::create_entity_with_init_n");
    addExtern<DAS_BIND_FUN(::create_entity)>(*this, lib, "create_entity", das::SideEffects::modifyExternal, "::create_entity");
    addExtern<DAS_BIND_FUN(::create_entity_n)>(*this, lib, "create_entity", das::SideEffects::modifyExternal, "::create_entity_n");
    addExtern<DAS_BIND_FUN(::create_entity_immediate_with_init)>(*this, lib, "create_entity_immediate", das::SideEffects::modifyExternal, "::create_entity_immediate_with_init");
    addExtern<DAS_BIND_FUN(::create_entity_immediate_with_init_n)>(*this, lib, "create_entity_immediate", das::SideEffects::modifyExternal, "::create_entity_immediate_with_init_n");
    addExtern<DAS_BIND_FUN(::create_entity_immediate)>(*this, lib, "create_entity_immediate", das::SideEffects::modifyExternal, "::create_entity_immediate");
    addExtern<DAS_BIND_FUN(::create_entity_immediate_n)>(*this, lib, "create_entity_immediate", das::SideEffects::modifyExternal, "::create_entity_immediate_n");
    addExtern<DAS_BIND_FUN(::create_entity_prefab)>(*this, lib, "create_entity_prefab", das::SideEffects::modifyExternal, "::create_entity_prefab");

    addInterop<::init_component, void, const das::TypeDeclPtr, int, ComponentInitializer&, const char*, vec4f>(*this, lib, "builtin_init", das::SideEffects::modifyArgumentAndExternal, "::init_component");
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