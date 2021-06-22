#include <iostream>
#include "listener.h"
void log(const char *str)
{
  str = str ? str : "nullptr";
  std::cout << str << std::endl;
}

static TestStruct t("a");
static TestStruct s("b");
extern "C" {
  
__declspec(dllexport) void f()
{
  static int c = 0;
  c++;
  std::cout << c << std::endl;
  log ("hahahaa");
}
}