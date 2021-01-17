

#include <iostream>
#include "CommonCode/Application/application.h"

int main(int argc, char** argv)
{
  Application application("Motion matching", 1848, 1016);

  application.main_loop();

  application.exit();
  
  return 0;
}