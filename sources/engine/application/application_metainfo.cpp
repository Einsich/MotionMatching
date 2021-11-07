#include "application_metainfo.h"
#include <fstream>

static ApplicationMetaInfo metaInfo;

template <class T>
std::istream& operator>>(std::istream& is, std::vector<T>& v) 
{
  char bracket;
  is >> bracket;
  T tmp;
  while (is >> tmp)
  {
    if (tmp == "]")
      return is;
    v.emplace_back(tmp);
  }
  is >> bracket;
  return is;
}

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
ApplicationMetaInfo &get_meta_info()
{
  return metaInfo;
}