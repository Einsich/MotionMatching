#pragma once
#include <ecs/type_registration.h>
#include <ecs/user_function_registration.h>
#include "user_function_registration.h"

#define ECS_REGISTER_TYPE(TYPE, NAME, TYPE_TIP, ...)\
ECS_TYPE_REGISTRATION(TYPE, NAME, TYPE_TIP, template_blk_constructor<TYPE>, __VA_ARGS__)\
ECS_USER_TYPE_REGISTRATION(TYPE, get_user_functions<TYPE>())

#define ECS_REGISTER_TYPE_AND_VECTOR(TYPE, NAME, TYPE_TIP, ...)\
ECS_REGISTER_TYPE(TYPE, NAME, TYPE_TIP, __VA_ARGS__)\
ECS_REGISTER_TYPE(ecs::vector<TYPE>, "vector<" NAME ">", ecs::TrivialMoveConstructor, __VA_ARGS__)