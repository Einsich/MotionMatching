#include "application/application_data.h"

extern size_t ecs_pull;

int main(int, char**)
{
  printf("ecs_pull %zu", ecs_pull);
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::filesystem::path root(currentPath.parent_path());

  Application application(currentPath.stem().string(), root.string(), 2048, 1024, true);
  application.start();
  application.main_loop();
  application.exit();
  
  return 0;
}