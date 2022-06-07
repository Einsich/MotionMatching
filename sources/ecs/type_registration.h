#pragma once
#include "manager/user_type_registration.h"
#include "manager/type_info.h"


#define ECS_REGISTER_TYPE(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
ecs::TypeInfo type_##VAR_NAME(ecs::get_rtti<TYPE>(), std::string(nameOf<TYPE>::value), TRIVIAL_COPY, TRIVIAL_MOVE, get_uset_type_info<TYPE>());

#define ECS_REGISTER_TYPE_AND_VECTOR(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
ECS_REGISTER_TYPE(VAR_NAME, TYPE, TRIVIAL_COPY, TRIVIAL_MOVE)\
ECS_REGISTER_TYPE(VAR_NAME##_vec, std::vector<TYPE>, false, true)