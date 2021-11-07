#include "init_game.inl"
//Code-generator production

void init_game_handler(const StartGameEvent &event);

ecs::EventDescription<StartGameEvent> init_game_descr("init_game", {
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, init_game_handler, ecs::SystemTag::Game);

void init_game_handler(const StartGameEvent &event)
{
  for (ecs::QueryIterator begin = init_game_descr.begin(), end = init_game_descr.end(); begin != end; ++begin)
  {
    init_game(
      event,
      *begin.get_component<SpriteFactory>(0),
      *begin.get_component<ScoreBoard>(1)
    );
  }
}


void init_game_singl_handler(const StartGameEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<StartGameEvent> init_game_singl_descr("init_game", {
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, init_game_singl_handler, ecs::SystemTag::Game);

void init_game_singl_handler(const StartGameEvent &event, ecs::QueryIterator &begin)
{
  init_game(
    event,
      *begin.get_component<SpriteFactory>(0),
      *begin.get_component<ScoreBoard>(1)
  );
}



