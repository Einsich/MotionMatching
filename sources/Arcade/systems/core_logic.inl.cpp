#include "core_logic.inl"
//Code-generator production

void controll_targets_func();

ecs::SystemDescription controll_targets_descr("controll_targets", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec2>("velocity"), false},
  {ecs::get_type_description<ecs::Tag>("target"), false}
}, controll_targets_func, ecs::SystemOrder::LOGIC, ecs::SystemTag::Game);

void controll_targets_func()
{
  for (ecs::QueryIterator begin = controll_targets_descr.begin(), end = controll_targets_descr.end(); begin != end; ++begin)
  {
    controll_targets(
      *begin.get_component<Transform2D>(0),
      *begin.get_component<vec2>(1)
    );
  }
}


void destroy_old_bullets_func();

ecs::SystemDescription destroy_old_bullets_descr("destroy_old_bullets", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<float>("creationTime"), false},
  {ecs::get_type_description<float>("lifeTime"), false},
  {ecs::get_type_description<ecs::Tag>("bullet"), false}
}, destroy_old_bullets_func, ecs::SystemOrder::LOGIC+1, ecs::SystemTag::Game);

void destroy_old_bullets_func()
{
  for (ecs::QueryIterator begin = destroy_old_bullets_descr.begin(), end = destroy_old_bullets_descr.end(); begin != end; ++begin)
  {
    destroy_old_bullets(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<float>(1),
      *begin.get_component<float>(2)
    );
  }
}


void move_hero_func();

ecs::SystemDescription move_hero_descr("move_hero", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec2>("velocity"), false},
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, move_hero_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game);

void move_hero_func()
{
  for (ecs::QueryIterator begin = move_hero_descr.begin(), end = move_hero_descr.end(); begin != end; ++begin)
  {
    move_hero(
      *begin.get_component<Transform2D>(0),
      *begin.get_component<vec2>(1),
      *begin.get_component<bool>(2)
    );
  }
}


void look_at_mouse_position_when_mouse_moves_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> look_at_mouse_position_when_mouse_moves_descr("look_at_mouse_position_when_mouse_moves", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, look_at_mouse_position_when_mouse_moves_handler, ecs::SystemTag::Game);

void look_at_mouse_position_when_mouse_moves_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = look_at_mouse_position_when_mouse_moves_descr.begin(), end = look_at_mouse_position_when_mouse_moves_descr.end(); begin != end; ++begin)
  {
    look_at_mouse_position_when_mouse_moves(
      event,
      *begin.get_component<WorldRenderer>(0),
      *begin.get_component<Transform2D>(1)
    );
  }
}


void fire_when_mouse_click_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> fire_when_mouse_click_descr("fire_when_mouse_click", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, fire_when_mouse_click_handler, ecs::SystemTag::Game);

void fire_when_mouse_click_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  for (ecs::QueryIterator begin = fire_when_mouse_click_descr.begin(), end = fire_when_mouse_click_descr.end(); begin != end; ++begin)
  {
    fire_when_mouse_click(
      event,
      *begin.get_component<WorldRenderer>(0),
      *begin.get_component<SpriteFactory>(1),
      *begin.get_component<Transform2D>(2),
      *begin.get_component<bool>(3)
    );
  }
}


void circle_attack_handler(const KeyDownEvent<SDLK_SPACE> &event);

ecs::EventDescription<KeyDownEvent<SDLK_SPACE>> circle_attack_descr("circle_attack", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, circle_attack_handler, ecs::SystemTag::Game);

void circle_attack_handler(const KeyDownEvent<SDLK_SPACE> &event)
{
  for (ecs::QueryIterator begin = circle_attack_descr.begin(), end = circle_attack_descr.end(); begin != end; ++begin)
  {
    circle_attack(
      event,
      *begin.get_component<Transform2D>(0),
      *begin.get_component<SpriteFactory>(1)
    );
  }
}


void look_at_mouse_position_when_mouse_moves_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> look_at_mouse_position_when_mouse_moves_singl_descr("look_at_mouse_position_when_mouse_moves", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, look_at_mouse_position_when_mouse_moves_singl_handler, ecs::SystemTag::Game);

void look_at_mouse_position_when_mouse_moves_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  look_at_mouse_position_when_mouse_moves(
    event,
      *begin.get_component<WorldRenderer>(0),
      *begin.get_component<Transform2D>(1)
  );
}


void fire_when_mouse_click_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> fire_when_mouse_click_singl_descr("fire_when_mouse_click", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, fire_when_mouse_click_singl_handler, ecs::SystemTag::Game);

void fire_when_mouse_click_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::QueryIterator &begin)
{
  fire_when_mouse_click(
    event,
      *begin.get_component<WorldRenderer>(0),
      *begin.get_component<SpriteFactory>(1),
      *begin.get_component<Transform2D>(2),
      *begin.get_component<bool>(3)
  );
}


void circle_attack_singl_handler(const KeyDownEvent<SDLK_SPACE> &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyDownEvent<SDLK_SPACE>> circle_attack_singl_descr("circle_attack", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, circle_attack_singl_handler, ecs::SystemTag::Game);

void circle_attack_singl_handler(const KeyDownEvent<SDLK_SPACE> &event, ecs::QueryIterator &begin)
{
  circle_attack(
    event,
      *begin.get_component<Transform2D>(0),
      *begin.get_component<SpriteFactory>(1)
  );
}



