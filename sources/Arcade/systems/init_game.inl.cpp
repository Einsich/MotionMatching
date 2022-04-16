#include "init_game.inl"
//Code-generator production

void init_game_handler(const StartGameEvent &event);
void init_game_singl_handler(const StartGameEvent &event, ecs::EntityId eid);

ecs::EventDescription<StartGameEvent> init_game_descr("init_game", {
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, {
}, {},
init_game_handler, init_game_singl_handler, ecs::SystemTag::Game);

void init_game_handler(const StartGameEvent &event)
{
  ecs::perform_event(event, init_game_descr, init_game);
}
void init_game_singl_handler(const StartGameEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_game_descr, eid, init_game);
}


