#include "coin_effect.inl"
//Code-generator production

void coin_move_func();

ecs::SystemDescription coin_move_descr("coin_move", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<vec3>("linear_velocity"), false},
  {ecs::get_type_description<vec3>("angular_velocity"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<float>("life_time"), false},
  {ecs::get_type_description<float>("life_period"), false},
  {ecs::get_type_description<ecs::Tag>("coinEffect"), false}
}, coin_move_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor,
{},
{});

void coin_move_func()
{
  ecs::perform_system(coin_move_descr, coin_move);
}

void spawn_coin_effect_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);
void spawn_coin_effect_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> spawn_coin_effect_descr("spawn_coin_effect", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false}
}, spawn_coin_effect_handler, spawn_coin_effect_singl_handler, ecs::SystemTag::GameEditor);

void spawn_coin_effect_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  ecs::perform_event(event, spawn_coin_effect_descr, spawn_coin_effect);
}
void spawn_coin_effect_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, spawn_coin_effect_descr, eid, spawn_coin_effect);
}


