#include <iostream>
#include "listener.h"
#include "Engine/time.h"
#include "config.h"
void log(const char *str)
{
  str = str ? str : "nullptr";
  int l = strlen(str);
  std::cout << str << l <<std::endl;
}

static TestStruct t("a");
static TestStruct s("b");
extern "C" {
  
__declspec(dllexport) void f()
{
  const char* m[1] ={"aaa"};
  add_configs(1, m);
  std::cout << Time::time() << std::endl;
  log ("hahahaa");
}
}