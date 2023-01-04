#include <daScript/daScript.h>
#include <application/time.h>
//#include <application/application.h>


class Test_ECS : public das::Module
{
public:
  Test_ECS() : Module("engine")
  {
    das::ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(get_delta_time)>(*this, lib, "get_delta_time", das::SideEffects::accessExternal, "get_delta_time");
    //addExtern<DAS_BIND_FUN(project_path_str)>(*this, lib, "project_path", das::SideEffects::accessExternal, "project_path_str");
    //addExtern<DAS_BIND_FUN(root_path_ctr)>(*this, lib, "root_path", das::SideEffects::accessExternal, "root_path_ctr");

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

REGISTER_MODULE(Test_ECS);