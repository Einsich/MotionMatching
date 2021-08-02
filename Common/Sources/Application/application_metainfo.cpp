#include "application_metainfo.h"
#include <fstream>
static ApplicationMetaInfo metaInfo;

void load_meta_info(const string &path)
{
  ifstream file(path, ios::in);
  string varName = "";
  while(file >> varName && varName != "{");
  while (file >> varName && varName != "}")
  {
    metaInfo.reflect([&](auto &arg, const char *name)
    { 
      if (name == varName)
      {
        file >> arg;
      }
    });
  }
}
void save_meta_info(const string &path)
{
  ofstream file(path, ios::out);
  string varName = "";
  file << "{\n";
  
  metaInfo.reflect([&](auto &arg, const char *name)
  { 
    file << name << " " << arg << "\n";
  });
  file << "}\n";
}
ApplicationMetaInfo &get_meta_info()
{
  return metaInfo;
}