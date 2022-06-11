#pragma once
#include "manager/user_type_registration.h"
#include "manager/type_info.h"
#include "manager/string_hash.h"
namespace ecs
{
  template<class T>
  void* template_constructor(void *memory)
  {
    return new (memory) T();
  }
  template<class T>
  void template_destructor(void *memory)
  {
    return ((T*)(memory))->~T();
  }
  template<typename T>
  void* template_copy_constructor(const void *src, void *dst)
  {
    return new (dst) T(*((T*)src));
  }
  template<typename T>
  void* template_move_constructor(void *src, void *dst)
  {
    return new (dst) T(std::move(*((T*)src)));
  }
  template<typename T>
  TypeRTTI get_rtti()
  {
    return TypeRTTI{ sizeof(T),
      ecs::template_constructor<T>,
      ecs::template_copy_constructor<T>,
      ecs::template_move_constructor<T>,
      ecs::template_destructor<T> };
  }
}
#define ECS_REGISTER_TYPE(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
namespace ecs{\
  ecs::TypeInfo type_##VAR_NAME(ecs::get_rtti<TYPE>(), std::string(#TYPE), TRIVIAL_COPY, TRIVIAL_MOVE, get_uset_type_info<TYPE>());\
  template<>\
  uint32_t type_hash<TYPE>()\
  { return HashedString(#TYPE); }\
  template<>\
  const char* type_name<TYPE>()\
  { return #TYPE; }\
}

#define ECS_REGISTER_TYPE_AND_VECTOR(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
ECS_REGISTER_TYPE(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
ECS_REGISTER_TYPE(VAR_NAME##_vec, vector<TYPE>, false, true)