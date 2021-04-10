#include "type_description.h"
namespace ecs
{

  FullTypeDescription::FullTypeDescription(std::string &&name, string_hash hash, uint size_of, void *(*constructor)(void*)):
  name(name), hash(hash), sizeOf(size_of), constructor(constructor)
  {
  }
  TypeDescription::TypeDescription(string_hash name_hash, uint typeId):
    name_hash(name_hash), typeId(typeId){}
  
  template<typename T>
  TypeDescription::TypeDescription(const char *name):
    name_hash(HashedString(name)), typeId(type_index<T>()){}
  
  string_hash TypeDescription::hash() const
  {
    return name_hash ^ (typeId * 16397816463u);
  }
  
}