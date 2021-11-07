#include <ecs.h>
#include <transform2d.h>
#include <input.h>
#include "game_structs.h"

template<typename Callable> void update_main_camera_position(Callable);

SYSTEM(ecs::SystemOrder::RENDER-1, ecs::Tag mainHero) update_camera_pos_before_render(
  const Transform2D &transform)
{
  vec2 heroPosition = transform.position;
  
  QUERY(mat3 cameraProjection) update_main_camera_position([heroPosition](Transform2D &transform)
  {
    transform.position = heroPosition;
  });
}


EVENT(mat3 cameraProjection) change_zoom(
  const MouseWheelEvent &event,
  Transform2D &transform,
  vec3 &zoom)
{
  zoom.z = glm::clamp(zoom.z - event.wheel * 0.02f, 0.f, 1.f);
  transform.scale = vec2(lerp(zoom.x, zoom.y, zoom.z));
}
