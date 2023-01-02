#include <daScript/daScript.h>
#include <application/time.h>


inline float get_delta_time()
{
  return Time::delta_time();
}

class Test_ECS : public das::Module
{
public:
  Test_ECS() : Module("engine")
  {
    das::ModuleLibrary lib;
    lib.addModule(this);
    lib.addBuiltInModule();

    addExtern<DAS_BIND_FUN(get_delta_time)>(*this, lib, "get_delta_time", das::SideEffects::accessExternal, "get_delta_time");

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