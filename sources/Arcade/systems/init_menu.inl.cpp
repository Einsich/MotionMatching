#include "init_menu.inl"
//Code-generator production

void start_game_button_func();

ecs::SystemDescription start_game_button_descr("start_game_button", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false},
  {ecs::get_type_description<ecs::Tag>("startGameButton"), false}
}, start_game_button_func, ecs::SystemOrder::UI, ecs::SystemTag::Game);

void start_game_button_func()
{
  ecs::perform_system(start_game_button_descr, start_game_button);
}
void exit_menu_button_func();

ecs::SystemDescription exit_menu_button_descr("exit_menu_button", {
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<int>("killsCount"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, exit_menu_button_func, ecs::SystemOrder::UI, ecs::SystemTag::Game);

void exit_menu_button_func()
{
  ecs::perform_system(exit_menu_button_descr, exit_menu_button);
}
void init_menu_handler(const StartMenuEvent &event);

ecs::EventDescription<StartMenuEvent> init_menu_descr("init_menu", {
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, init_menu_handler, ecs::SystemTag::Game);

void init_menu_handler(const StartMenuEvent &event)
{
  ecs::perform_event(event, init_menu_descr, init_menu);
}


void init_menu_singl_handler(const StartMenuEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<StartMenuEvent> init_menu_singl_descr("init_menu", {
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, init_menu_singl_handler, ecs::SystemTag::Game);

void init_menu_singl_handler(const StartMenuEvent &event, ecs::QueryIterator &begin)
{
  init_menu(
    event,
      *begin.get_component<WorldRenderer, 0>()
  );
}



