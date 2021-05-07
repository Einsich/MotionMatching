#include "Engine/input.h"
#include "person_controller.h"
#include "../Animation/animation_player.h"
#include "Engine/transform.h"
#include "Engine/Render/debug_arrow.h"
PersonController::PersonController(vec3 position) :
simulatedRotation(0), realRotation(0), wantedRotation(0), angularSpeed(0),
speed(0),
simulatedPosition(position), realPosition(position),
disableEvents(false),
crouching(false),
rotationStrafe(0)
{}
  constexpr float maxErrorRadius = 0.5f;

float rotation_abs(float rotation_delta)
{
  rotation_delta = abs(rotation_delta);
  rotation_delta -= (int)(rotation_delta/PITWO)*PITWO;
  
  rotation_delta = rotation_delta > PI ? PITWO - rotation_delta : rotation_delta;
  return rotation_delta;
}
void PersonController::update_from_speed(const AnimationPlayer &player, Transform &transform, vec3 speed_set, float dt)
{
  speed = speed_set;
  float nextRotation = realRotation - player.rootDeltaRotation * dt;
  
  if (rotation_abs(nextRotation - wantedRotation) < rotation_abs(realRotation - wantedRotation))
  {
    realRotation = nextRotation; 
    //simulatedRotation = realRotation;
  }

  simulatedPosition += glm::rotateY(speed * dt, -realRotation);
  
  realPosition = transform.get_position() -
  (player.rootDeltaTranslation.z * transform.get_forward() + 
  player.rootDeltaTranslation.y * transform.get_up()+ 
  -player.rootDeltaTranslation.x * transform.get_right()) * dt;

  vec3 positionDelta = simulatedPosition - realPosition;
  float errorRadius = length2(positionDelta);
  if (errorRadius > maxErrorRadius * maxErrorRadius)
  {
    errorRadius = sqrt(errorRadius);
    realPosition += positionDelta * (errorRadius-maxErrorRadius)/errorRadius;
  }
  transform.get_position() = realPosition;
  transform.set_rotation(-realRotation); 
  
  draw_transform(transform);

}
void PersonController::set_pos_rotation(Transform &transform, vec3 position, float rotation)
{
  realPosition = simulatedPosition = position;
  realRotation = simulatedRotation = rotation;
  transform.get_position() = position;
  transform.set_rotation(realRotation); 
}