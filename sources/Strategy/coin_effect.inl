#include <ecs.h>
#include <render/render.h> 
#include <transform.h> 
#include <application/time.h>
#include <input.h>
#include <application/application.h>
#include <camera.h>
#include "heightmap.h"

SYSTEM(scene=game, editor, ecs::Tag coinEffect) coin_move(
  ecs::EntityId eid,
  vec3 &linear_velocity,
  vec3 &angular_velocity,
  Transform &transform,
  float &life_time,
  float life_period
)
{
  float dt = Time::delta_time();
  transform.get_position() += linear_velocity * dt;
  linear_velocity += vec3(0, -10, 0) * dt;
  float dr = angular_velocity.length();
  if (dr > 0)
  {
    transform.set_rotation(glm::rotate(dr * Time::time() , angular_velocity / dr));
  }
  life_time += dt;
  if (life_period < life_time)
    ecs::destroy_entity(eid);
}



EVENT(scene=game, editor) spawn_coin_effect(
  const MouseButtonDownEvent<MouseButton::LeftButton> &event,
  const MainCamera &mainCamera,
  const HeightMap &heightMap)
{
  //if (event.action != MouseAction::Down)
    return;
  float x = event.x, y = event.y;

  auto [w, h] = get_resolution();
  
  vec2 screenPos((x / w * 2.f - 1.0f)*mainCamera.aspectRatio, 1.0f - y / h * 2.f);
  vec3 p = mainCamera.transform * vec4(screenPos, 1, 1);
  vec3 n = mainCamera.transform * vec4(screenPos, 1, 0);
  n = normalize(n);
  vec3 worldPos;
  if (heightMap.ray_trace(p, n, worldPos))
  {
    const ecs::Template *t = ecs::get_template("coin_effect");

    for (int i = 0; i < 25; i++)
    {
      ecs::ComponentInitializerList list;
      list.set("transform", Transform(worldPos, vec3(0), vec3(0.1f)));
      list.set("linear_velocity", (vec3(0,3.5,0)+ rand_vec3()) * 1.5f);
      list.set("angular_velocity", rand_vec3() * 1.5f);
      ecs::create_entity(t, std::move(list));
    }
  }
}