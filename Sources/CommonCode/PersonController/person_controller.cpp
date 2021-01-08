#include "person_controller.h"
#include "../Time/time.h"
TestPersonController::TestPersonController(GameObjectPtr character, AnimationPlayerPtr animation):
  character(character), animation(animation), rotation(0)
{

}

void TestPersonController::update()
{
  vec2 axis = vec2(Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s), Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a));
  float r = Input::input().get_key(SDLK_q) - Input::input().get_key(SDLK_e);

  float speed = 100 * (1.f + 1.f * Input::input().get_key(SDLK_LSHIFT));
  axis *= speed * Time::delta_time();
  r *= DegToRad * 0.25f;
  rotation += r;
  Transform &t = character->get_transform();
  t.get_position() += axis.x * t.get_forward() + axis.y * t.get_right();
  t.set_rotation(rotation); 
}