
#include <iostream>
#include "Application/application.h"
#include "config.h"

int main(int argc, char** argv)
{
  add_configs(argc, (const char**)(argv));
  
  Application application(get_config("project"), 1848, 1016, true);
  application.start();
  application.main_loop();

  application.exit();
  
  return 0;
}