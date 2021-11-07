#include "application/application_data.h"

int main(int, char**)
{
  std::filesystem::path currentPath = std::filesystem::current_path();
  std::filesystem::path root(currentPath.parent_path());

  Application application(currentPath.stem().string(), root.string(), 2048, 1024, true);
  application.start();
  application.main_loop();
  application.exit();
  
  return 0;
}