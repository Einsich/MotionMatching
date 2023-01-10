#pragma once
#include <ecs/type_registration.h>
#include <ecs/user_function_registration.h>
#include "user_function_registration.h"


template <typename T>
struct RegistrationHelper
{
  struct CustomTypeFabric final : public ecs::DefaultTypeFabric<T>
  {
    CustomTypeFabric(ecs::Type type, bool has_awaiter = false):
      ecs::DefaultTypeFabric<T>(type, true, has_awaiter)
    {}

    void prefab_constructor(void *raw_memory, const ecs::ComponentPrefab &prefab, bool r_value) const override
    {
      return template_blk_constructor<T>(raw_memory, prefab, r_value);
    }
  } fabric;

  RegistrationHelper(const char *name, ecs::Type type) : fabric(type)
  {
    ecs::type_registration<T>(name, &fabric);
  }
};

#define ECS_REGISTER_TYPE(TYPE, NAME, TYPE_TIP)\
static RegistrationHelper<TYPE> __CONCAT__(registrator_helper, __COUNTER__) (NAME, TYPE_TIP);\
ECS_USER_TYPE_REGISTRATION(TYPE, get_user_functions<TYPE>())

#define ECS_REGISTER_TYPE_AND_VECTOR(TYPE, NAME, TYPE_TIP)\
ECS_REGISTER_TYPE(TYPE, NAME, TYPE_TIP)\
ECS_REGISTER_TYPE(ecs::vector<TYPE>, "vector<" NAME ">", ecs::TrivialMoveConstructor)