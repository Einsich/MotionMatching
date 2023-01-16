#include "init_game.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache init_game__cache__;

static void init_game_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const StartGameEvent &>(event), init_game__cache__, init_game);
}

static void init_game_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const StartGameEvent &>(event), init_game__cache__, init_game);
}

static void registration_pull_init_game()
{
  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/init_game.inl:6",
  "init_game",
  &init_game__cache__,
  {
    {"sf", ecs::get_type_index<SpriteFactory>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<SpriteFactory>()},
    {"sb", ecs::get_type_index<ScoreBoard>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ScoreBoard>()}
  },
  {},
  {},
  {},
  {},
  {},
  &init_game_handler, &init_game_single_handler,
  ecs::EventIndex<StartGameEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_init_game)
ECS_PULL_DEFINITION(variable_pull_init_game)
