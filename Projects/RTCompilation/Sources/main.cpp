#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <functional>
#include <filesystem>
#include <vector>
#include <stdarg.h>
#include "dll_loader.h"
#include "listener.h"
typedef unsigned int uint;

void check_dl_error()
{
  const char* dlsym_error=dlerror();
  if(dlsym_error != NULL)  {
    std::cerr << dlsym_error << std::endl;
    exit(EXIT_FAILURE);
  }
}
void *dynlib = nullptr;
std::function<void()> func;
std::function<void(Listener*)> listener_setter;

void unload_test_lib()
{
  if (dynlib != nullptr)
  {
    dlclose(dynlib);
    check_dl_error();
    dynlib = nullptr;
    func = nullptr;
  }
}
void compile_test_lib()
{
  system("cmake -G Ninja -DPROJECT=TestDll -DBUILD_TYPE=dev -B ../../../Builds/win/TestDll/dev -S ../../../Projects/RTCompilation/dllSources"
  "&& cd D:/MotionMatching/Builds/win/TestDll/dev"
  "&& ninja");
}

void load_test_lib()
{
  dynlib = dlopen("D:/MotionMatching/Builds/win/TestDll/dev/TestDll-dev.dll", 0);

  check_dl_error();
  void* f = dlsym(dynlib, "f");
  check_dl_error();
  func = reinterpret_cast<void(*)()> (f);
  f = dlsym(dynlib, "set_instance");
  check_dl_error();
  listener_setter = reinterpret_cast<void(*)(Listener*)> (f);
}

int main(int , char** )
{
  compile_test_lib();
  load_test_lib();
  int command;
  Listener listener;
  set_instance(&listener);
  while(std::cin >> command && command != 0)
  {
    if (command == 1)
    {
      unload_test_lib();
      compile_test_lib();
      load_test_lib();
      listener_setter(&listener);
      for (TestStruct &t : Listener::instance().structs)
      {
        printf("[%s]\n", t.name.c_str());
      }
      func();
    }
  }
  std::cout << "end\n";
}
