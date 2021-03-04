

#include <iostream>
#include "CommonCode/Application/application.h"
#include "CommonCode/Application/config.h"

int main(int argc, char** argv)
{
  add_configs(argc - 1, (const char**)(argv + 1));
  Application application("Motion matching", 1848, 1016);

  application.main_loop();

  application.exit();
  
  return 0;
}