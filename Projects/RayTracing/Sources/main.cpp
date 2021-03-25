#include <iostream>
#include "Application/application.h"
#include "Application/config.h"
#include "ray_tracing_scene.h"

int main(int argc, char** argv)
{
  add_configs(argc, (const char**)argv);
  
  Application application(get_config("project"), 1848, 1016, true);
  application.get_scene().init(init_scene);
  application.main_loop();

  application.exit();
  
  return 0;
}