#include <daScript/daScript.h>
#include <ecs.h>

using namespace das;

void register_system(const char *name, const das::Array &args)
{

  debug_log("system %s", name);
  const char **data = (const char **)args.data;
  for (int i = 0, n = args.size; i < n; i++)
  {
    debug_log("    %s", data[i]);
  }
}

class Module_ECS : public Module {
public:
  Module_ECS() : Module("ecs_impl") {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(register_system)>(*this, lib, "register_system", SideEffects::accessExternal, "register_system");

    verifyAotReady();
  }
  virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
    // specifying which include files are required for this module
    //tw << "#include \"tutorial02aot.h\"\n";
    // specifying AOT type, in this case direct cpp mode (and not hybrid mode)
    return ModuleAotType::cpp;
  }
};

REGISTER_MODULE(Module_ECS);