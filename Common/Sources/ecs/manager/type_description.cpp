#include "type_description.h"
namespace ecs
{

  FullTypeDescription::FullTypeDescription(std::string &&name, string_hash hash, uint size_of,
   Constructor constructor, CopyConstructor copy_constructor):
  name(name), hash(hash), sizeOf(size_of), constructor(constructor), copy_constructor(copy_constructor)
  {
  }
  TypeDescription::TypeDescription(string_hash name_hash, uint typeId):
    name_hash(name_hash), typeId(typeId){}
  
  
  string_hash TypeDescription::hash() const
  {
    return name_hash ^ (typeId * 16397816463u);
  }
  
}