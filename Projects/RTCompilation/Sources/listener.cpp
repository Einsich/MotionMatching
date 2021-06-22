#include "listener.h"

TestStruct::TestStruct(const char *name):name(name)
{
  printf("Hello %s\n", name);
  Listener::instance().structs.push_back(*this);
};
TestStruct::~TestStruct()
{
  printf("Goodbye %s\n", name.c_str());
};

static Listener *listener = nullptr;

Listener &Listener::instance()
{
  if (!listener)
    listener = new Listener();
  return *listener;
}
__declspec(dllexport)void set_instance(Listener *instance)
{
  if (listener)
  {
    instance->structs.insert(instance->structs.end(), listener->structs.begin(), listener->structs.end());
    delete listener;
  }
  listener = instance;
}