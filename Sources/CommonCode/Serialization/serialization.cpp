#include "serialization.h"
#include "../Time/time_scope.h"
#include <fstream>
#include <sstream>

void print_file_size(size_t fileSize)
{
  double s = 0;
  char* b = nullptr;
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
  debug_log("file size = %.1f %s", s, b);  
}
size_t save_object(const ISerializable &object, const std::string &path)
{
  TimeScope scope("save file " + path);
  ofstream file (join_recources_path(path), ios::binary);
  size_t fileSize = object.serialize(file);
  scope.stop();
  print_file_size(fileSize);
  file.close();
  return fileSize;
}

size_t load_object(ISerializable &object, const std::string &path)
{
  TimeScope scope("load file " + path);
  ifstream file(join_recources_path(path), ios::binary);
  size_t fileSize = object.deserialize(file);
  scope.stop();
  print_file_size(fileSize);
  file.close();
  return fileSize;
}