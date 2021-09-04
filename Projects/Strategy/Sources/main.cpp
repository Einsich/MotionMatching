#include "Application/application.h"
#include "config.h"
#include "ecs/ecs_scene.h"

int main(int argc, char** argv)
{
  add_configs(argc, (const char**)(argv));
  
  Application application(get_config("project"), 2048, 1024, true);
  application.start();
  application.main_loop();
  application.exit();
  
  return 0;
}