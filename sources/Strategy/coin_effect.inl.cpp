#include "coin_effect.inl"
#include <ecs_perform.h>
//Code-generator production

void coin_move_func();

ecs::SystemDescription coin_move_descr("coin_move", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("linear_velocity"), false},
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("angular_velocity"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("life_time"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("life_period"), false},
  {-1u, ecs::get_name_hash("coinEffect"), false}
}, {
},
{},
{},
coin_move_func, "act", {}, false);

void coin_move_func()
{
  ecs::perform_system(coin_move_descr, coin_move);
}

void spawn_coin_effect_handler(const ecs::Event &event);
void spawn_coin_effect_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription spawn_coin_effect_descr(
  ecs::get_mutable_event_handlers<MouseButtonDownEvent<MouseButton::LeftButton>>(), "spawn_coin_effect", {
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false},
  {ecs::get_type_hash<HeightMap>(), ecs::get_name_hash("heightMap"), false}
}, {
},
{},
{},
spawn_coin_effect_handler, spawn_coin_effect_singl_handler, {});

void spawn_coin_effect_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseButtonDownEvent<MouseButton::LeftButton>&)event, spawn_coin_effect_descr, spawn_coin_effect);
}
void spawn_coin_effect_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseButtonDownEvent<MouseButton::LeftButton>&)event, spawn_coin_effect_descr, eid, spawn_coin_effect);
}


