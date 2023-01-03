#include "init_menu.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache start_game_button__cache__;

static ecs::QueryCache exit_menu_button__cache__;

static ecs::QueryCache setup_camera__cache__;

static void start_game_button_implementation()
{
  ecs::perform_system(start_game_button__cache__, start_game_button);
}

static void exit_menu_button_implementation()
{
  ecs::perform_system(exit_menu_button__cache__, exit_menu_button);
}

static void setup_camera_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), setup_camera__cache__, setup_camera);
}

static void setup_camera_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), setup_camera__cache__, setup_camera);
}

static void registration_pull_init_menu()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "start_game_button",
  &start_game_button__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"sb", ecs::get_type_index<ScoreBoard>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ScoreBoard>()}
  },
  {
    {"startGameButton", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  "imgui_render",
  {},
  {},
  {},
  &start_game_button_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "exit_menu_button",
  &exit_menu_button__cache__,
  {
    {"isWinner", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"killsCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"sb", ecs::get_type_index<ScoreBoard>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ScoreBoard>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &exit_menu_button_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "setup_camera",
  &setup_camera__cache__,
  {
    {"cameraProjection", ecs::get_type_index<mat3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat3>()},
    {"zoom", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<WorldRenderer>()}
  },
  {},
  {},
  {},
  {},
  {},
  &setup_camera_handler, &setup_camera_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_init_menu)
ECS_PULL_DEFINITION(variable_pull_init_menu)
