#include "ecs/ecs.h"
#include "PersonController/third_person_controller.h"
#include "Engine/transform.h"
#include "Animation/animation_player.h"
#include "Engine/time.h"
#include "Animation/man_property.h"
#include "PersonController/person_controller.h"


constexpr float lerp_strength = 4.f;
constexpr float zoom_strength = 0.4f;

vec3 rotation_to_orientation(vec2 rotation)
{
  float x = rotation.x;
  float y = rotation.y;
  return vec3(cos(y) * cos(x), sin(y), cos(y) * sin(x));
}
float clamp_zoom(float zoom)
{
  return glm::clamp(zoom, 0.5f, 10.f);
}

EVENT() third_controller_appear(
  const ecs::OnEntityCreated &,
  ThirdPersonController &thirdPersonController)
{
  thirdPersonController.wantedCameraOrientation = thirdPersonController.currentCameraOrientation =
      rotation_to_orientation(thirdPersonController.currentCameraRotation);

  thirdPersonController.currentZoom  = thirdPersonController.wantedZoom = clamp_zoom(thirdPersonController.currentZoom);
}

template<typename Callable>
void update_attached_camera(const ecs::EntityId&, Callable);

SYSTEM(ecs::SystemOrder::LOGIC) third_peson_controller_update(
  ecs::EntityId attachedCamera,
  PersonController &personController,
  ThirdPersonController &thirdPersonController) 
{
  
  float dt = Time::delta_time();
  thirdPersonController.currentCameraRotation =
  lerp(thirdPersonController.currentCameraRotation,
    thirdPersonController.wantedCameraRotation + vec2(PI * 0.5f * thirdPersonController.view_offset, 0),
    dt * lerp_strength);
  thirdPersonController.currentZoom = lerp(thirdPersonController.currentZoom, thirdPersonController.wantedZoom, dt * lerp_strength);
  thirdPersonController.currentCameraOrientation = rotation_to_orientation(thirdPersonController.currentCameraRotation);

  QUERY() update_attached_camera(attachedCamera, [&](Transform &transform)
  {
    vec3 hipsPoint = vec3(0, ManProperty::instance->hipsHeightStand, 0);


    transform.get_position() = personController.realPosition + hipsPoint - thirdPersonController.currentCameraOrientation * thirdPersonController.currentZoom;
    transform.set_rotation(PI * 0.5f - thirdPersonController.currentCameraRotation.x, -thirdPersonController.currentCameraRotation.y);
  });
}

EVENT() mouse_move_handler(
  const MouseMoveEvent &e,
  ecs::EntityId eid,
  ThirdPersonController &thirdPersonController,
  PersonController &personController)
{
  if (personController.disableEvents)
    return;
  float const pixToRad = PI / 180.f * 0.2f;
  thirdPersonController.wantedCameraRotation += vec2(e.dx, -e.dy) * pixToRad;

  thirdPersonController.wantedCameraRotation.y = glm::clamp(thirdPersonController.wantedCameraRotation.y, -PI * 0.45f, -PI * 0.1f);
  thirdPersonController.wantedCameraOrientation = rotation_to_orientation(thirdPersonController.wantedCameraRotation);
  ecs::send_event_immediate(eid, ControllerMouseMoveEvent(e));
}

EVENT() mouse_wheel_handler(
  const MouseWheelEvent &e,
  ThirdPersonController &thirdPersonController)
{
  thirdPersonController.wantedZoom = clamp_zoom(thirdPersonController.wantedZoom + -e.wheel * zoom_strength);
}

EVENT() view_offset_handler(
  const KeyboardEvent &e,
  ThirdPersonController &thirdPersonController)
{
  switch (e.keycode)
  {
    case SDLK_LEFT:thirdPersonController.view_offset = 3; break;
    case SDLK_UP:thirdPersonController.view_offset = 2; break;
    case SDLK_RIGHT:thirdPersonController.view_offset = 1; break;
    case SDLK_DOWN:thirdPersonController.view_offset = 0; break;
  
  }
}

EVENT() crouch_event_handler(
  const KeyboardEvent &e,
  ecs::EntityId eid,
  ThirdPersonController &thirdPersonController)
{
  ecs::send_event_immediate(eid, ControllerKeyBoardEvent(e));
}
EVENT() animation_player_handler(
  const KeyboardEvent &e,
  ThirdPersonController &thirdPersonController,
  AnimationPlayer &animationPlayer)
{
  int d = 0;
  if(e.keycode == SDLK_m)
    d = 1;
  if(e.keycode == SDLK_n)
    d = -1;
  if (d != 0)
  {
    int n = animationPlayer.index.get_data_base()->clips.size();
    int clip = animationPlayer.index.first.get_clip_index();
    clip = (clip + d + n) %n;
    animationPlayer.index.first = animationPlayer.index.second = AnimationIndex(animationPlayer.index.get_data_base(), clip, 0);
    debug_log("Play anim %s", animationPlayer.index.first.get_clip().name.c_str());
  }
}