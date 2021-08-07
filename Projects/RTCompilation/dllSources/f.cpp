#include <iostream>
#include "listener.h"
#include <assert.h>
#include "Engine/time.h"
#include <ecs/ecs.h>
void log(const char *str)
{
  assert(strcmp(str, "null"));
  str = str ? str : "nullptr";
  std::cout << str << Time::time() <<std::endl;
}

static TestStruct t("a");
static TestStruct s("b");
ecs::QueryDescription d("aaa", {}, false);
 extern "C" {
  
__declspec(dllexport) void f()
{

  const char* m[1] ={"aaa"};
  log ("hahahaa");
}
}