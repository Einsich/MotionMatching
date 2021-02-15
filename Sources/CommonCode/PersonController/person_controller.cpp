#include "person_controller.h"
#include "../Time/time.h"

TestPersonController::TestPersonController(GameObjectPtr character, AnimationPlayerPtr animation):
  character(character), animation(animation), rotation(0)
{

}

void TestPersonController::update()
{
  float speed = Input::input().get_key(SDLK_w) * (1.f + 2*Input::input().get_key(SDLK_LSHIFT)) * 1.2f;
  const float skipSecond = (AnimationPathFeature::SkipCadres * AnimationPathFeature::PathLength) / 30.f;
  float rotationSpeed = 60 * DegToRad;
  float rotationDelta = wantedRotation - rotation;
  
  if (glm::abs(rotationDelta) > skipSecond * rotationSpeed)
    rotationDelta = glm::clamp(rotationDelta, -skipSecond * rotationSpeed, skipSecond * rotationSpeed);
  
  if (glm::abs(animation->rootDeltaRotation) > glm::abs(rotationDelta / skipSecond))
    rotation +=  rotationDelta / skipSecond * Time::delta_time();
  else
    rotation += animation->rootDeltaRotation * Time::delta_time();
  animation->inputGoal.tags.clear();
  if (Input::input().get_key(SDLK_SPACE) > 0)
    animation->inputGoal.tags.push_back(AnimationTag::Jump);
    else
  animation->inputGoal.tags.push_back(crouching ? AnimationTag::Crouch : AnimationTag::Stay);
  
  animation->inputGoal.path.rotation = rotationDelta;

  for (int i = 0; i < AnimationPathFeature::PathLength; i++)
    animation->inputGoal.path.path[i] = vec3(0, 0, 1) * speed * (i + 1.f) * (float)AnimationPathFeature::SkipCadres / 30.f;
  Transform &t = character->get_transform();
  t.get_position() -= (animation->rootDeltaTranslation.z * t.get_forward() + animation->rootDeltaTranslation.x * t.get_right()) * Time::delta_time() ;
  t.set_rotation(rotation); 
  
  //animation->get_state_machine().set_rotation(r);
  //animation->get_state_machine().set_speed(speed);
}

void TestPersonController::rotate(const KeyboardEvent &event)
{
  float r = DegToRad * 90.f;
  if (event.keycode == SDLK_a)
    wantedRotation += r;
  if (event.keycode == SDLK_d)
    wantedRotation -= r;
}
void TestPersonController::crouch(const KeyboardEvent &event)
{
  crouching = !crouching;
  //animation->get_state_machine().crouch(crouching);
}
void TestPersonController::jump(const KeyboardEvent &event)
{
  //animation->get_state_machine().jump();

}