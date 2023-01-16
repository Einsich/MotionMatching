#include "kills_statistic.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache gather_all_targets__cache__;

static ecs::QueryCache show_kill_stat__cache__;

static ecs::QueryCache check_winner__cache__;

static ecs::QueryCache collect_kills__cache__;

template<typename Callable>
static void gather_all_targets(Callable lambda)
{
  ecs::perform_query<>(gather_all_targets__cache__, lambda);
}

static void show_kill_stat_implementation()
{
  ecs::perform_system(show_kill_stat__cache__, show_kill_stat);
}

static void check_winner_implementation()
{
  ecs::perform_system(check_winner__cache__, check_winner);
}

static void collect_kills_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KillTargetEvent &>(event), collect_kills__cache__, collect_kills);
}

static void collect_kills_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KillTargetEvent &>(event), collect_kills__cache__, collect_kills);
}

static void registration_pull_kills_statistic()
{
  ecs::register_query(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/kills_statistic.inl:30",
  "gather_all_targets",
  &gather_all_targets__cache__,
  {},
  {
    {"target", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  );

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/kills_statistic.inl:14",
  "show_kill_stat",
  &show_kill_stat__cache__,
  {
    {"killsCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {},
  &show_kill_stat_implementation);

  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/kills_statistic.inl:25",
  "check_winner",
  &check_winner__cache__,
  {
    {"isWinner", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  "act",
  {},
  {},
  {},
  &check_winner_implementation);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/kills_statistic.inl:6",
  "collect_kills",
  &collect_kills__cache__,
  {
    {"killsCount", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &collect_kills_handler, &collect_kills_single_handler,
  ecs::EventIndex<KillTargetEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_kills_statistic)
ECS_PULL_DEFINITION(variable_pull_kills_statistic)
