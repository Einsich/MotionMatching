#include "serialization.h"
#include "common.h"

void print_file_size(const std::string &path, size_t fileSize)
{
  constexpr int N = 4;
  float bound[N] = {1e3, 1e6, 1e9, 1e12};
  std::string suffix[N] = {"bytes", "Kb", "Mb", "Gb"};
  for (int i = 0; i < N; i++)
    if (fileSize < bound[i])
    {
      double size = fileSize / (1e3 * bound[i]);
      debug_log("file %s size = %.1f %s", path.c_str(), size, suffix[i].c_str()); 
      return;
    }
}
