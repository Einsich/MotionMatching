#include "init_game.inl"
#include <ecs_perform.h>
//Code-generator production

void init_game_handler(const ecs::Event &event);
void init_game_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_game_descr(
  ecs::get_mutable_event_handlers<StartGameEvent>(), "init_game", {
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, {
}, {},
{},
{},
init_game_handler, init_game_singl_handler, ecs::tags::all);

void init_game_handler(const ecs::Event &event)
{
  ecs::perform_event((const StartGameEvent&)event, init_game_descr, init_game);
}
void init_game_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const StartGameEvent&)event, init_game_descr, eid, init_game);
}


