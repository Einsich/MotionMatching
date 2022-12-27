#include "core_logic.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache controll_targets__cache__;

static ecs::QueryCache destroy_old_bullets__cache__;

static ecs::QueryCache move_hero__cache__;

static ecs::QueryCache look_at_mouse_position_when_mouse_moves__cache__;

static ecs::QueryCache fire_when_mouse_click__cache__;

static ecs::QueryCache circle_attack__cache__;

static void controll_targets_implementation()
{
  ecs::perform_system(controll_targets__cache__, controll_targets);
}

static void destroy_old_bullets_implementation()
{
  ecs::perform_system(destroy_old_bullets__cache__, destroy_old_bullets);
}

static void move_hero_implementation()
{
  ecs::perform_system(move_hero__cache__, move_hero);
}

static void look_at_mouse_position_when_mouse_moves_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), look_at_mouse_position_when_mouse_moves__cache__, look_at_mouse_position_when_mouse_moves);
}

static void look_at_mouse_position_when_mouse_moves_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), look_at_mouse_position_when_mouse_moves__cache__, look_at_mouse_position_when_mouse_moves);
}

static void fire_when_mouse_click_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), fire_when_mouse_click__cache__, fire_when_mouse_click);
}

static void fire_when_mouse_click_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), fire_when_mouse_click__cache__, fire_when_mouse_click);
}

static void circle_attack_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyDownEvent<SDLK_SPACE> &>(event), circle_attack__cache__, circle_attack);
}

static void circle_attack_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyDownEvent<SDLK_SPACE> &>(event), circle_attack__cache__, circle_attack);
}

static void registration_pull_core_logic()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "controll_targets",
  &controll_targets__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"velocity", ecs::get_type_index<vec2>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec2>()}
  },
  {
    {"target", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &controll_targets_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "destroy_old_bullets",
  &destroy_old_bullets__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"creationTime", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"lifeTime", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {
    {"bullet", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &destroy_old_bullets_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "move_hero",
  &move_hero__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"velocity", ecs::get_type_index<vec2>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec2>()},
    {"isWinner", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &move_hero_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "look_at_mouse_position_when_mouse_moves",
  &look_at_mouse_position_when_mouse_moves__cache__,
  {
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<WorldRenderer>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &look_at_mouse_position_when_mouse_moves_handler, &look_at_mouse_position_when_mouse_moves_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "fire_when_mouse_click",
  &fire_when_mouse_click__cache__,
  {
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<WorldRenderer>()},
    {"sf", ecs::get_type_index<SpriteFactory>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<SpriteFactory>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"isWinner", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &fire_when_mouse_click_handler, &fire_when_mouse_click_single_handler),
  ecs::EventIndex<MouseButtonDownEvent<MouseButton::LeftButton>>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "circle_attack",
  &circle_attack__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"sf", ecs::get_type_index<SpriteFactory>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<SpriteFactory>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &circle_attack_handler, &circle_attack_single_handler),
  ecs::EventIndex<KeyDownEvent<SDLK_SPACE>>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_core_logic)
ECS_PULL_DEFINITION(variable_pull_core_logic)
