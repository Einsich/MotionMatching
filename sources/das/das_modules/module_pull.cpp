#include <daScript/daScript.h>

void require_common_das_modules();
void require_game_das_modules();


void require_project_specific_modules()
{
  das::daScriptEnvironment::bound->das_def_tab_size = 2;
  require_common_das_modules();
  require_game_das_modules();
}