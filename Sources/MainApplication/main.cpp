

#include <iostream>
#include "CommonCode/Application/application.h"

int main(int argc, char** argv)
{
  Application application("Motion matching", 1280, 720);
   
  application.main_loop();

  application.exit();
  
  return 0;
}