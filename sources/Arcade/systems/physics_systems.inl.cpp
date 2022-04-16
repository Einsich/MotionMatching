#include "physics_systems.inl"
//Code-generator production

ecs::QueryDescription gather_all_target_colliders_descr("gather_all_target_colliders", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<bool>("destroyed"), false},
  {ecs::get_type_description<ecs::Tag>("target"), false}
}, {
});

template<typename Callable>
void gather_all_target_colliders(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, Transform2D&, bool&>
  (gather_all_target_colliders_descr, lambda);
}


void collision_detection_func();

ecs::SystemDescription collision_detection_descr("collision_detection", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("bullet"), false}
}, {
}, {},
collision_detection_func, ecs::stage::act, ecs::tags::all,
{},
{});

void collision_detection_func()
{
  ecs::perform_system(collision_detection_descr, collision_detection);
}


