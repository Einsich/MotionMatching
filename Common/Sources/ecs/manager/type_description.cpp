#include "type_description.h"
#include "common.h"
namespace ecs
{
  FullTypeDescription::FullTypeDescription():
  name("bRoKeN"), hash(0), sizeOf(0),
  copy_constructor(), destructor()
  {
  }
  FullTypeDescription::FullTypeDescription(const char *type, const char *name, string_hash hash, uint size_of,
   CopyConstructor copy_constructor, Destructor destructor):
  type(type),
  name(name),
  hash(hash), sizeOf(size_of),
  copy_constructor(copy_constructor), destructor(destructor)
  {
    if(destructor == nullptr)
    {
      debug_error("Hasn't copy constructor for %s %s", type, name);
    }
    if(copy_constructor == nullptr)
    {
      debug_error("Hasn't destructor for %s %s", type, name);
    }
  }

  TypeDescription::TypeDescription(string_hash name_hash, uint typeId):
    typeHash(hash(name_hash, typeId)){}
  
  
  uint TypeDescription::type_hash() const 
  {
    return typeHash;
  }  
  
  bool TypeDescription::operator==(const TypeDescription &other)
  {
    return typeHash == other.typeHash;
  }
}