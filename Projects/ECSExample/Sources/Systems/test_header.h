#pragma once
#include <string>

struct A
{
  std::string x = "It's A";
  A() {printf("(");};
  A(const A &a) {printf("(");};
  ~A() {printf(")");};
  A(const std::string &s):
  x(std::move(s)){printf("(");}
};
struct B
{
  std::string x = "It's B";
  B() {printf("[");};
  B(const B &a) {printf("[");};
  ~B() {printf("]");};
  B(const std::string &s):
  x(std::move(s)){printf("[");}
};
class C
{

};
class D
{

};
struct MyEvent {int x;};
