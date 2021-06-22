#pragma once
#include <vector>
#include <string>
struct TestStruct
{
  std::string name;
  TestStruct(const char *name);
  ~TestStruct();
};
class Listener
{
public:
  static Listener &instance();
  std::vector<TestStruct> structs;
};
extern "C" __declspec(dllexport) void set_instance(Listener *instance);