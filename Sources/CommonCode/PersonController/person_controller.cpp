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
  float wantedRotation = -r * DegToRad * 0.05f;
  if (glm::abs(animation->rootDeltaRotation * Time::delta_time()) < glm::abs(wantedRotation))
  rotation += wantedRotation;
  else
  rotation += animation->rootDeltaRotation * Time::delta_time();
  Transform &t = character->get_transform();
  t.get_position() -= (animation->rootDeltaTranslation.z * t.get_forward() + animation->rootDeltaTranslation.x * t.get_right()) * Time::delta_time() ;
  t.set_rotation(rotation); 
  
  animation->set_rotation(r);
  animation->set_speed(speed);
}
void TestPersonController::crouch(const KeyboardEvent &event)
{
  crouching = !crouching;
  animation->crouch(crouching);
}
void TestPersonController::jump(const KeyboardEvent &event)
{
  animation->jump();

}