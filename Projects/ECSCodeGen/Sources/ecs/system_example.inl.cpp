#include "system_example.inl"
#include "ecs_core.h"

ecs::SystemDescription test_system_descr = {{{ecs::get_type_description<int>("a"), false}, {ecs::get_type_description<float>("b"), false}}};
ecs::SystemDescription test_system_descr = {{{ecs::get_type_description<std::string>("s"), false}, {ecs::get_type_description<float>("f"), false}}};
