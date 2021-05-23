#include "animation_player.inl"
//Code-generator production

void animation_player_update_func();

ecs::SystemDescription animation_player_update_descr("animation_player_update", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<AnimationRender>("animationRender"), false}
}, animation_player_update_func, ecs::SystemOrder::LOGIC);

void animation_player_update_func()
{
  for (ecs::QueryIterator begin = animation_player_update_descr.begin(), end = animation_player_update_descr.end(); begin != end; ++begin)
  {
    animation_player_update(
      *begin.get_component<Transform>(0),
      *begin.get_component<AnimationPlayer>(1),
      *begin.get_component<AnimationRender>(2)
    );
  }
}



