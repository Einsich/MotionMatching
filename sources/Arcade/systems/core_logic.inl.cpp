#include "core_logic.inl"
//Code-generator production

void controll_targets_func();

ecs::SystemDescription controll_targets_descr("controll_targets", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec2>("velocity"), false},
  {ecs::get_type_description<ecs::Tag>("target"), false}
}, {
}, {},
controll_targets_func, ecs::SystemOrder::LOGIC, ecs::SystemTag::Game,
{},
{});

void controll_targets_func()
{
  ecs::perform_system(controll_targets_descr, controll_targets);
}

void destroy_old_bullets_func();

ecs::SystemDescription destroy_old_bullets_descr("destroy_old_bullets", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<float>("creationTime"), false},
  {ecs::get_type_description<float>("lifeTime"), false},
  {ecs::get_type_description<ecs::Tag>("bullet"), false}
}, {
}, {},
destroy_old_bullets_func, ecs::SystemOrder::LOGIC+1, ecs::SystemTag::Game,
{},
{});

void destroy_old_bullets_func()
{
  ecs::perform_system(destroy_old_bullets_descr, destroy_old_bullets);
}

void move_hero_func();

ecs::SystemDescription move_hero_descr("move_hero", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec2>("velocity"), false},
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
move_hero_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game,
{},
{});

void move_hero_func()
{
  ecs::perform_system(move_hero_descr, move_hero);
}

void look_at_mouse_position_when_mouse_moves_handler(const MouseMoveEvent &event);
void look_at_mouse_position_when_mouse_moves_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseMoveEvent> look_at_mouse_position_when_mouse_moves_descr("look_at_mouse_position_when_mouse_moves", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
look_at_mouse_position_when_mouse_moves_handler, look_at_mouse_position_when_mouse_moves_singl_handler, ecs::SystemTag::Game);

void look_at_mouse_position_when_mouse_moves_handler(const MouseMoveEvent &event)
{
  ecs::perform_event(event, look_at_mouse_position_when_mouse_moves_descr, look_at_mouse_position_when_mouse_moves);
}
void look_at_mouse_position_when_mouse_moves_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, look_at_mouse_position_when_mouse_moves_descr, eid, look_at_mouse_position_when_mouse_moves);
}

void fire_when_mouse_click_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);
void fire_when_mouse_click_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> fire_when_mouse_click_descr("fire_when_mouse_click", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
fire_when_mouse_click_handler, fire_when_mouse_click_singl_handler, ecs::SystemTag::Game);

void fire_when_mouse_click_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  ecs::perform_event(event, fire_when_mouse_click_descr, fire_when_mouse_click);
}
void fire_when_mouse_click_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, fire_when_mouse_click_descr, eid, fire_when_mouse_click);
}

void circle_attack_handler(const KeyDownEvent<SDLK_SPACE> &event);
void circle_attack_singl_handler(const KeyDownEvent<SDLK_SPACE> &event, ecs::EntityId eid);

ecs::EventDescription<KeyDownEvent<SDLK_SPACE>> circle_attack_descr("circle_attack", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
circle_attack_handler, circle_attack_singl_handler, ecs::SystemTag::Game);

void circle_attack_handler(const KeyDownEvent<SDLK_SPACE> &event)
{
  ecs::perform_event(event, circle_attack_descr, circle_attack);
}
void circle_attack_singl_handler(const KeyDownEvent<SDLK_SPACE> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, circle_attack_descr, eid, circle_attack);
}


