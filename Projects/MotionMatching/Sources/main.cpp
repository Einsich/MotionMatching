

#include <iostream>
#include "Application/application.h"
#include "Application/config.h"
#include "motion_matching_scene.h"

int main(int argc, char** argv)
{
  add_configs(argc - 1, (const char**)(argv + 1));
  
  Application application(get_config("project"), 1848, 1016, true);
  application.get_scene().init(init_scene);
  application.main_loop();

  application.exit();
  
  return 0;
}