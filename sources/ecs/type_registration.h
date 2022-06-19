#pragma once
#include "config/user_type_registration.h"
#include "manager/type_info.h"
#include "manager/string_hash.h"
namespace ecs
{
  template<class T>
  void template_constructor(void *memory)
  {
    new (memory) T();
  }
  template<class T>
  void template_destructor(void *memory)
  {
    ((T*)(memory))->~T();
  }
  template<typename T>
  void template_copy_constructor(const void *src, void *dst)
  {
    new (dst) T(*((T*)src));
  }
  template<typename T>
  void template_move_constructor(void *src, void *dst)
  {
    new (dst) T(std::move(*((T*)src)));
  }
  template<typename T>
  TypeRTTI get_rtti(bool trivialCopy=false, bool trivialMove=false, bool trivialConstruct=false, bool trivialDestruct=false)
  {
    return TypeRTTI{ sizeof(T),
      ecs::template_constructor<T>,
      ecs::template_copy_constructor<T>,
      ecs::template_move_constructor<T>,
      ecs::template_destructor<T>,
      trivialCopy,
      trivialMove,
      trivialConstruct,
      trivialDestruct
    };
  }
}

//bool trivialCopy, bool trivialMove, bool trivialConstruct, bool trivialDestruct
#define ECS_REGISTER_TYPE(VAR_NAME, TYPE, ...)\
namespace ecs{\
  ecs::TypeInfo type_##VAR_NAME(ecs::get_rtti<TYPE>(__VA_ARGS__), ecs::string(#TYPE), get_uset_type_info<TYPE>());\
  template<>\
  uint32_t type_hash<TYPE>()\
  { return HashedString(#TYPE); }\
  template<>\
  const char* type_name<TYPE>()\
  { return #TYPE; }\
  template<>\
  const ecs::TypeInfo &type_info<TYPE>()\
  { return type_##VAR_NAME; }\
}


//bool trivialCopy, bool trivialMove, bool trivialConstruct, bool trivialDestruct
#define ECS_REGISTER_TYPE_AND_VECTOR(VAR_NAME, TYPE, ...)\
ECS_REGISTER_TYPE(VAR_NAME, TYPE, __VA_ARGS__)\
ECS_REGISTER_TYPE(VAR_NAME##_vec, vector<TYPE>, false, true, false, false)
