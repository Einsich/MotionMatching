#include "physics_systems.inl"
//Code-generator production

ecs::QueryDescription gather_all_target_colliders_descr("gather_all_target_colliders", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<bool>("destroyed"), false},
  {ecs::get_type_description<ecs::Tag>("target"), false}
});

template<typename Callable>
void gather_all_target_colliders(Callable lambda)
{
  for (ecs::QueryIterator begin = gather_all_target_colliders_descr.begin(), end = gather_all_target_colliders_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<Transform2D>(1),
      *begin.get_component<bool>(2)
    );
  }
}


void move_all_entity_with_velocity_func();

ecs::SystemDescription move_all_entity_with_velocity_descr("move_all_entity_with_velocity", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec2>("velocity"), false},
  {ecs::get_type_description<float>("rotationVelocity"), true}
}, move_all_entity_with_velocity_func, ecs::SystemOrder::LOGIC+1, ecs::SystemTag::Game);

void move_all_entity_with_velocity_func()
{
  for (ecs::QueryIterator begin = move_all_entity_with_velocity_descr.begin(), end = move_all_entity_with_velocity_descr.end(); begin != end; ++begin)
  {
    move_all_entity_with_velocity(
      *begin.get_component<Transform2D>(0),
      *begin.get_component<vec2>(1),
       begin.get_component<float>(2)
    );
  }
}


void collision_detection_func();

ecs::SystemDescription collision_detection_descr("collision_detection", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("bullet"), false}
}, collision_detection_func, ecs::SystemOrder::LOGIC, ecs::SystemTag::Game);

void collision_detection_func()
{
  for (ecs::QueryIterator begin = collision_detection_descr.begin(), end = collision_detection_descr.end(); begin != end; ++begin)
  {
    collision_detection(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<Transform2D>(1)
    );
  }
}



