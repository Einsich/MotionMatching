#include <ecs.h>
#include <application/time.h>
#include <transform2d.h>
#include "game_structs.h"


SYSTEM(ecs::SystemOrder::LOGIC+1) move_all_entity_with_velocity(
  Transform2D &transform,
  const vec2 &velocity,
  float *rotationVelocity
)
{
  float dt = Time::delta_time();
  transform.position += velocity * dt;
  transform.rotation += (rotationVelocity ? *rotationVelocity : 0) * dt;
}

template<typename Callable> void gather_all_target_colliders(Callable);


SYSTEM(ecs::SystemOrder::LOGIC, ecs::Tag bullet) collision_detection(
  ecs::EntityId eid,
  const Transform2D &transform
)
{
  vec2 bulletPosition = transform.position;
  float bulletRadius = transform.scale.x * 0.5f;
  bool penetrate = false;
  QUERY(ecs::Tag target) gather_all_target_colliders([&](
    ecs::EntityId eid,
    const Transform2D &transform,
    bool &destroyed)
  {
    if (!destroyed)
    {
      vec2 targetPosition = transform.position;
      float targetRadius = transform.scale.x * 0.5f;
      float dist = length(targetPosition - bulletPosition);
      if (dist < bulletRadius + targetRadius)
      {
        ecs::send_event<KillTargetEvent>(KillTargetEvent());
        ecs::destroy_entity(eid);
        penetrate = true;
        destroyed = true;
      }
    }
  });
  if (penetrate)
    ecs::destroy_entity(eid);
}
