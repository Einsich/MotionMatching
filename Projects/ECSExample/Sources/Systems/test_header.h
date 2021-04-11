#pragma once
#include <string>

struct A
{
  std::string x = "It's A";
  A() {};
  ~A() {};
  A(const std::string &s):
  x(std::move(s)){}
};
struct B
{
  std::string x = "It's B";
  B() {};
  ~B() {};
  B(const std::string &s):
  x(std::move(s)){}
};
class C
{

};
class D
{

};
struct MyEvent {int x;};
