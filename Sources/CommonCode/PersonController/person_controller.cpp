#include "person_controller.h"
#include "../Time/time.h"

TestPersonController::TestPersonController(GameObjectPtr character, AnimationPlayerPtr animation):
  character(character), animation(animation), rotation(0)
{

}

void TestPersonController::update()
{
  float speed = Input::input().get_key(SDLK_w) * (1.f + Input::input().get_key(SDLK_LSHIFT));
  float r = Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a);
  float wantedRotation = -r * DegToRad *40.f* Time::delta_time();
  if (glm::abs(animation->rootDeltaRotation * Time::delta_time()) < glm::abs(wantedRotation))
  rotation += wantedRotation;
  else
  rotation += animation->rootDeltaRotation * Time::delta_time();


  animation->inputGoal.path.rotation = wantedRotation;
  for (int i = 0; i < AnimationPathFeature::PathLength; i++)
    animation->inputGoal.path.path[i] = vec3(0, 0, 1) * speed * (i + 1.f) * (float)AnimationPathFeature::SkipCadres / 30.f;
  Transform &t = character->get_transform();
  t.get_position() -= (animation->rootDeltaTranslation.z * t.get_forward() + animation->rootDeltaTranslation.x * t.get_right()) * Time::delta_time() ;
  t.set_rotation(rotation); 
  
  animation->get_state_machine().set_rotation(r);
  animation->get_state_machine().set_speed(speed);
}
void TestPersonController::crouch(const KeyboardEvent &event)
{
  crouching = !crouching;
  animation->get_state_machine().crouch(crouching);
}
void TestPersonController::jump(const KeyboardEvent &event)
{
  animation->get_state_machine().jump();

}