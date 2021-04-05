#include "person_controller.h"
#include "../Animation/animation_player.h"
#include "Transform/transform.h"
#include "GameObject/game_object.h"
#include "math.h"
#include "Components/DebugTools/debug_arrow.h"
PersonController::PersonController(vec3 position) :
simulatedRotation(0), realRotation(0),
speed(0),
simulatedPosition(position), realPosition(position)
{}
  constexpr float maxErrorRadius = 0.5f;

void PersonController::update_from_speed(vec3 speed_set, float dt)
{
  speed = speed_set;
  update(dt);
}
float rotation_abs(float rotation_delta)
{
  rotation_delta = abs(rotation_delta);
  if (rotation_delta >= PITWO)
    rotation_delta -= PITWO;
  return rotation_delta;
}
void PersonController::update(float dt)
{
  REQUIRE(AnimationPlayer, player);
  REQUIRE(Transform, transform);
  float nextRotation = realRotation + player->rootDeltaRotation * dt;
  
  if (rotation_abs(nextRotation - simulatedRotation) < rotation_abs(realRotation - simulatedRotation))
  {
    realRotation = nextRotation; 
    simulatedRotation = realRotation;
  }

  simulatedPosition += glm::rotateY(speed * dt, PIHALF + realRotation);
  
  realPosition = transform->get_position() -
  (player->rootDeltaTranslation.z * transform->get_forward() + 
  player->rootDeltaTranslation.y * transform->get_up()+ 
  -player->rootDeltaTranslation.x * transform->get_right()) * dt;

  vec3 positionDelta = simulatedPosition - realPosition;
  float errorRadius = length2(positionDelta);
  if (errorRadius > maxErrorRadius * maxErrorRadius)
  {
    errorRadius = sqrt(errorRadius);
    realPosition += positionDelta * (errorRadius-maxErrorRadius)/errorRadius;
  }
  transform->get_position() = realPosition;
  transform->set_rotation(PIHALF + realRotation); 
  
  draw_transform(*transform);

}
void PersonController::set_pos_rotation(vec3 position, float rotation)
{
  realPosition = simulatedPosition = position;
  realRotation = simulatedRotation = rotation - PIHALF;
  REQUIRE(Transform, transform);
  transform->get_position() = position;
  transform->set_rotation(realRotation); 
}