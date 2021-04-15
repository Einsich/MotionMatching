

#include <iostream>
#include "Application/application.h"
#include "Application/config.h"
#include "ecs/ecs_scene.h"

int main(int argc, char** argv)
{
  add_configs(argc, (const char**)(argv));
  
  ecs::Scene ecsScene;
  Application application((IScene*)&ecsScene, get_config("project"), 1848, 1016, true);
  application.start();
  application.main_loop();

  application.exit();
  
  return 0;
}