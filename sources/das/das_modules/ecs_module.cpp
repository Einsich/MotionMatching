
#include <ecs/ecs.h>
#include "ecs_module.h"

MAKE_EXTERNAL_TYPE_FACTORY(Function, das::Function)
MAKE_EXTERNAL_TYPE_FACTORY(ExprBlock, das::ExprBlock)


class Module_ECS : public das::Module
{
public:
  Module_ECS() : Module("ecs_impl")
  {
    das::ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(::register_system)>(*this, lib, "register_system", das::SideEffects::modifyExternal, "register_system");
    addExtern<DAS_BIND_FUN(::register_query)>(*this, lib, "register_query", das::SideEffects::modifyExternal, "register_query");
    addExtern<DAS_BIND_FUN(::perform_query)>(*this, lib, "query", das::SideEffects::modifyExternal, "perform_query");

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