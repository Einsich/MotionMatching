#include "physics_systems.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_all_target_colliders_descr("gather_all_target_colliders", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("destroyed"), false},
  {-1u, ecs::get_name_hash("target"), false}
}, {
});

template<typename Callable>
void gather_all_target_colliders(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, Transform2D&, bool&>
  (gather_all_target_colliders_descr, lambda);
}


void move_all_entity_with_velocity_func();

ecs::SystemDescription move_all_entity_with_velocity_descr("move_all_entity_with_velocity", {
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<vec2>(), ecs::get_name_hash("velocity"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("rotationVelocity"), true}
}, {
},
{},
{},
move_all_entity_with_velocity_func, "act", {}, false);

void move_all_entity_with_velocity_func()
{
  ecs::perform_system(move_all_entity_with_velocity_descr, move_all_entity_with_velocity);
}

void collision_detection_func();

ecs::SystemDescription collision_detection_descr("collision_detection", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {-1u, ecs::get_name_hash("bullet"), false}
}, {
},
{},
{},
collision_detection_func, "act", {}, false);

void collision_detection_func()
{
  ecs::perform_system(collision_detection_descr, collision_detection);
}


