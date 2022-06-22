#include "coin_effect.inl"
#include <ecs_perform.h>
//Code-generator production

void coin_move_func();

ecs::SystemDescription coin_move_descr("coin_move", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<vec3>("linear_velocity"), false},
  {ecs::get_type_description<vec3>("angular_velocity"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<float>("life_time"), false},
  {ecs::get_type_description<float>("life_period"), false}
}, {
}, {"game","editor","ecs::Tag coinEffect"},
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
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false}
}, {
}, {"game","editor"},
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


