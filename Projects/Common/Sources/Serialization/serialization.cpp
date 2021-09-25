#include "serialization.h"

void print_file_size(const std::string &path, size_t fileSize)
{
  double s = 0;
  string b = "";
  if (fileSize < 1e3)
  {
    s = fileSize;
    b = "bytes";
  } else
  if (fileSize < 1e6)
  {
    s = fileSize / 1e3;
    b = "Kb";
  } else
  if (fileSize < 1e9)
  {
    s = fileSize / 1e6;
    b = "Mb";
  } else
  {
    s = fileSize / 1e9;
    b = "Gb";
  }
  //debug_log("file %s size = %.1f %s", path.c_str(), s, b.c_str());  
}