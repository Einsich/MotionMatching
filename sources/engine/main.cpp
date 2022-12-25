#include "application/application_data.h"
extern size_t x;
extern size_t y;
extern size_t z;

int main(int, char**)
{
  //size_t w = x + y + z;
  //printf("%d", w);
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::filesystem::path root(currentPath.parent_path());

  Application application(currentPath.stem().string(), root.string(), 2048, 1024, true);
  application.start();
  application.main_loop();
  application.exit();
  
  return 0;
}