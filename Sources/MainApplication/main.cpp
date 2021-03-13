

#include <iostream>
#include "CommonCode/Application/application.h"
#include "CommonCode/Application/config.h"
#include "motion_matching_scene.h"

int main(int argc, char** argv)
{
  add_configs(argc - 1, (const char**)(argv + 1));
  Application application("Motion matching", 1848, 1016, true);
  application.get_scene().init(init_scene);
  application.main_loop();

  application.exit();
  
  return 0;
}