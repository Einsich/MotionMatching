#include "physics.h"
#include <vector>
#include "common.h"
#include "Transform/transform.h"
struct ColliderGO
{
  Transform* transform;
  ColliderPtr collider;
  ColliderGO(Transform* transform, ColliderPtr collider) : transform(transform), collider(collider){}
};

static vector<ColliderGO> colliders;


void add_collider(GameObjectPtr go, ColliderPtr collider)
{
  if (go)
  {
    Transform* transform = go->get_component<Transform>();
    if (transform && collider)
      colliders.push_back(ColliderGO(transform, collider));
    else
    {
      debug_error("There isn't a Transform at game object");
    }    
  }
  else
  {
    debug_error("Empty game object");
  } 
}

Collision ray_cast(const Ray &ray)
{
  Collision result;
  for (const ColliderGO & collider : colliders)
  {
    Collision collision;
    
    if (collider.collider->raycast_hit(collision, collider.transform->get_transform(), collider.transform->get_rotation(), ray) && collision.distance < result.distance)
    {
      result = collision;
      result.collider = collider.collider;
      result.gameObject = collider.collider->game_object();
    }
  }
  return result;
}