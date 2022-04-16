#include "init_menu.inl"
#include <ecs_perform.h>
//Code-generator production

void start_game_button_func();

ecs::SystemDescription start_game_button_descr("start_game_button", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false},
  {ecs::get_type_description<ecs::Tag>("startGameButton"), false}
}, {
}, {},
start_game_button_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void start_game_button_func()
{
  ecs::perform_system(start_game_button_descr, start_game_button);
}

void exit_menu_button_func();

ecs::SystemDescription exit_menu_button_descr("exit_menu_button", {
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<int>("killsCount"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, {
}, {},
exit_menu_button_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void exit_menu_button_func()
{
  ecs::perform_system(exit_menu_button_descr, exit_menu_button);
}

void setup_camera_handler(const ecs::OnEntityCreated &event);
void setup_camera_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> setup_camera_descr("setup_camera", {
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, {
}, {},
setup_camera_handler, setup_camera_singl_handler, ecs::tags::all);

void setup_camera_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, setup_camera_descr, setup_camera);
}
void setup_camera_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, setup_camera_descr, eid, setup_camera);
}


