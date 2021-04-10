#pragma once
#include <string>

struct A
{
  std::string x = "It's A";
  A(){printf("[A]%s\n", x.c_str());}
};
struct B
{
  std::string x = "It's B";
  B(){printf("[B]%s\n", x.c_str());}
};
class C
{

};
class D
{

};
