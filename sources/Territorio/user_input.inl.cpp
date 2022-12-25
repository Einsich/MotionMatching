#include "user_input.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache is_game_started__cache__;

static ecs::QueryCache select_map_query__cache__;

static ecs::QueryCache start_game__cache__;

static ecs::QueryCache select_spawn_point__cache__;

static ecs::QueryCache select_invasion__cache__;

static ecs::QueryCache change_invasion_weight__cache__;

static ecs::QueryCache check_mouse_over_ui__cache__;

template<typename Callable>
static void is_game_started(Callable lambda)
{
  ecs::perform_query<bool&>(is_game_started__cache__, lambda);
}

template<typename Callable>
static void select_map_query(Callable lambda)
{
  ecs::perform_query<const MapArrays&>(select_map_query__cache__, lambda);
}

static void start_game_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyDownEvent<SDLK_RETURN> &>(event), start_game__cache__, start_game);
}

static void start_game_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyDownEvent<SDLK_RETURN> &>(event), start_game__cache__, start_game);
}

static void select_spawn_point_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseButtonDownEvent<MouseButton::RightButton> &>(event), select_spawn_point__cache__, select_spawn_point);
}

static void select_spawn_point_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseButtonDownEvent<MouseButton::RightButton> &>(event), select_spawn_point__cache__, select_spawn_point);
}

static void select_invasion_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), select_invasion__cache__, select_invasion);
}

static void select_invasion_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), select_invasion__cache__, select_invasion);
}

static void change_invasion_weight_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), change_invasion_weight__cache__, change_invasion_weight);
}

static void change_invasion_weight_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), change_invasion_weight__cache__, change_invasion_weight);
}

static void check_mouse_over_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), check_mouse_over_ui__cache__, check_mouse_over_ui);
}

static void check_mouse_over_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), check_mouse_over_ui__cache__, check_mouse_over_ui);
}

static void registration_pull_user_input()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "is_game_started",
  &is_game_started__cache__,
  {
    {"gameStarted", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "select_map_query",
  &select_map_query__cache__,
  {
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MapArrays>()}
  },
  {},
  {}
  ));

  ecs::register_event(ecs::EventDescription(
  "",
  "start_game",
  &start_game__cache__,
  {},
  {
    {"isPlayer", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &start_game_handler, &start_game_single_handler),
  ecs::EventIndex<KeyDownEvent<SDLK_RETURN>>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "select_spawn_point",
  &select_spawn_point__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()}
  },
  {
    {"player_spawning", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &select_spawn_point_handler, &select_spawn_point_single_handler),
  ecs::EventIndex<MouseButtonDownEvent<MouseButton::RightButton>>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "select_invasion",
  &select_invasion__cache__,
  {
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<uint>()},
    {"neighbors", ecs::get_type_index<ecs::vector<ecs::EntityId>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<ecs::EntityId>>()},
    {"neighborsIdx", ecs::get_type_index<ecs::vector<uint>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<uint>>()},
    {"invasions", ecs::get_type_index<ecs::vector<Invasion>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<Invasion>>()},
    {"invasion_weight", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {
    {"isPlayer", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &select_invasion_handler, &select_invasion_single_handler),
  ecs::EventIndex<MouseButtonDownEvent<MouseButton::LeftButton>>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "change_invasion_weight",
  &change_invasion_weight__cache__,
  {
    {"invasion_weight", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<float>()},
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()}
  },
  {
    {"isPlayer", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &change_invasion_weight_handler, &change_invasion_weight_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "check_mouse_over_ui",
  &check_mouse_over_ui__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &check_mouse_over_ui_handler, &check_mouse_over_ui_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_user_input)
