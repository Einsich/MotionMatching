#include <daScript/daScript.h>
//#include <daScript/../../src/builtin/module_builtin_ast.h>
#include <ecs.h>

MAKE_EXTERNAL_TYPE_FACTORY(Function,das::Function)

using namespace das;

static das::ContextPtr sharedContext;
ecs::vector<ecs::string> to_ecs_string_array(const das::Array &a)
{
  ecs::vector<ecs::string> v;
  v.resize(a.size);
  const char **data = (const char **)a.data;
  for (int i = 0, n = a.size; i < n; i++)
  {
    v[i] = data[i];
    debug_log("%s", data[i]);
  }

  return v;
}

ecs::vector<ecs::FunctionArgument> to_ecs_arg_array(const das::Array &a)
{
  ecs::vector<ecs::FunctionArgument> v;
  v.resize(a.size);
  struct Arg
  {
    const char *type;
    const char *name;
  };
  const Arg *data = (const Arg*)a.data;
  for (int i = 0, n = a.size; i < n; i++)
  {
    uint typeHash = data[i].type ? ecs::get_name_hash(data[i].type) : -1u;
    uint nameHash = ecs::get_name_hash(data[i].name);

    v[i] = ecs::FunctionArgument{typeHash, nameHash, false};
    debug_log("%s %s", data[i].type, data[i].name);
  }

  return v;
}
void register_system(
  const char *name,
  const char *stage,
  const das::Array &require_args,
  const das::Array &require_not_args,
  const das::Array &before,
  const das::Array &after,
  const das::Array &tags,
  const das::FunctionPtr &function
  )
{
  debug_log("system %s", name);

  sharedContext->call(function.get(), nullptr, 0);

      //ecs::SystemDescription f(
      name,
      to_ecs_arg_array(require_args),
      to_ecs_arg_array(require_not_args),
      to_ecs_string_array(before),
      to_ecs_string_array(after),
      stage,
      to_ecs_string_array(tags);
      false;
}

class Module_ECS : public Module {
public:
  Module_ECS() : Module("ecs_impl") {
    ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(register_system)>(*this, lib, "register_system", SideEffects::accessExternal, "register_system");

    verifyAotReady();
    sharedContext = das::make_shared<das::Context>(4 * 1024); 
  }
  virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
    // specifying which include files are required for this module
    //tw << "#include \"tutorial02aot.h\"\n";
    // specifying AOT type, in this case direct cpp mode (and not hybrid mode)
    return ModuleAotType::cpp;
  }
};

REGISTER_MODULE(Module_ECS);