#include <daScript/misc/platform.h>
#include <daScript/daScriptModule.h>
#include <daScript/daScript.h>


void require_project_specific_modules() {

  das::daScriptEnvironment::bound->das_def_tab_size = 2;
  NEED_MODULE(Module_ECS);
  NEED_MODULE(Test_ECS);
}
