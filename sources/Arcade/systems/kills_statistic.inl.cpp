#include "kills_statistic.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_all_targets_descr("gather_all_targets", {
  {ecs::get_type_description<ecs::Tag>("target"), false}
}, {
});

template<typename Callable>
void gather_all_targets(Callable lambda)
{
  ecs::perform_query<>
  (gather_all_targets_descr, lambda);
}


void show_kill_stat_func();

ecs::SystemDescription show_kill_stat_descr("show_kill_stat", {
  {ecs::get_type_description<int>("killsCount"), false}
}, {
}, {},
show_kill_stat_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void show_kill_stat_func()
{
  ecs::perform_system(show_kill_stat_descr, show_kill_stat);
}

void check_winner_func();

ecs::SystemDescription check_winner_descr("check_winner", {
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
check_winner_func, ecs::stage::act, ecs::tags::all,
{},
{});

void check_winner_func()
{
  ecs::perform_system(check_winner_descr, check_winner);
}

void collect_kills_handler(const KillTargetEvent &event);
void collect_kills_singl_handler(const KillTargetEvent &event, ecs::EntityId eid);

ecs::EventDescription<KillTargetEvent> collect_kills_descr("collect_kills", {
  {ecs::get_type_description<int>("killsCount"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, {
}, {},
collect_kills_handler, collect_kills_singl_handler, ecs::tags::all);

void collect_kills_handler(const KillTargetEvent &event)
{
  ecs::perform_event(event, collect_kills_descr, collect_kills);
}
void collect_kills_singl_handler(const KillTargetEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, collect_kills_descr, eid, collect_kills);
}


