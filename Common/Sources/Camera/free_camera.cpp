#include "free_camera.h"
#include "Event/input.h"
#include "Time/time.h"

FreeCamera::FreeCamera(vec3 position, vec2 rotation):
curRotation(rotation), wantedRotation(rotation), curPosition(position), wantedPosition(position), rotationEnable(false)
{ 
  transform.set_position(curPosition);
  calculate_transform();
}
void FreeCamera::calculate_transform()
{
  if (!isMainCamera)
    return;
  transform.set_rotation(PI*0.5f-curRotation.x, -curRotation.y, 0);
}
void FreeCamera::mouse_move_handler(const MouseMoveEvent &e)
{
  if (!isMainCamera)
    return;
  if (rotationEnable)
  {
    float const pixToRad = PI / 180.f * 0.2f;
    wantedRotation += vec2(e.dx, e.dy) * pixToRad;

  }
}
void FreeCamera::mouse_click_handler(const MouseClickEvent &e)
{
  if (!isMainCamera)
    return;
  if (e.buttonType == MouseButtonType::MiddleButton)
  {
    rotationEnable = e.action == MouseButtonAction::Down;
  }
}

void FreeCamera::update()
{
  if (!isMainCamera)
    return;
  
  vec3 delta = -transform.get_forward() * (Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s)) +
              transform.get_right() * (Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a)) +
              transform.get_up() * (Input::input().get_key(SDLK_e) - Input::input().get_key(SDLK_c));
  const float minSpeed = 5.6f;
  const float maxSpeed = 20.6f;
  float speed = lerp(minSpeed, maxSpeed, Input::input().get_key(SDLK_LSHIFT));
  wantedPosition += delta * speed * Time::delta_time();

  float lerpFactor = Time::delta_time() * 3;
  curRotation = lerp(curRotation, wantedRotation, lerpFactor);
  curPosition = lerp(curPosition, wantedPosition, lerpFactor);
  transform.get_position() = curPosition;
  calculate_transform();
}
