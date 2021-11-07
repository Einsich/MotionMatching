#include "kills_statistic.inl"
//Code-generator production

ecs::QueryDescription gather_all_targets_descr("gather_all_targets", {
  {ecs::get_type_description<ecs::Tag>("target"), false}
});

template<typename Callable>
void gather_all_targets(Callable lambda)
{
  for (ecs::QueryIterator begin = gather_all_targets_descr.begin(), end = gather_all_targets_descr.end(); begin != end; ++begin)
  {
    lambda(
    );
  }
}


void show_kill_stat_func();

ecs::SystemDescription show_kill_stat_descr("show_kill_stat", {
  {ecs::get_type_description<int>("killsCount"), false}
}, show_kill_stat_func, ecs::SystemOrder::UI, ecs::SystemTag::Game);

void show_kill_stat_func()
{
  for (ecs::QueryIterator begin = show_kill_stat_descr.begin(), end = show_kill_stat_descr.end(); begin != end; ++begin)
  {
    show_kill_stat(
      *begin.get_component<int>(0)
    );
  }
}


void check_winner_func();

ecs::SystemDescription check_winner_descr("check_winner", {
  {ecs::get_type_description<bool>("isWinner"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, check_winner_func, ecs::SystemOrder::LOGIC, ecs::SystemTag::Game);

void check_winner_func()
{
  for (ecs::QueryIterator begin = check_winner_descr.begin(), end = check_winner_descr.end(); begin != end; ++begin)
  {
    check_winner(
      *begin.get_component<bool>(0)
    );
  }
}


void collect_kills_handler(const KillTargetEvent &event);

ecs::EventDescription<KillTargetEvent> collect_kills_descr("collect_kills", {
  {ecs::get_type_description<int>("killsCount"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, collect_kills_handler, ecs::SystemTag::Game);

void collect_kills_handler(const KillTargetEvent &event)
{
  for (ecs::QueryIterator begin = collect_kills_descr.begin(), end = collect_kills_descr.end(); begin != end; ++begin)
  {
    collect_kills(
      event,
      *begin.get_component<int>(0)
    );
  }
}


void collect_kills_singl_handler(const KillTargetEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KillTargetEvent> collect_kills_singl_descr("collect_kills", {
  {ecs::get_type_description<int>("killsCount"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, collect_kills_singl_handler, ecs::SystemTag::Game);

void collect_kills_singl_handler(const KillTargetEvent &event, ecs::QueryIterator &begin)
{
  collect_kills(
    event,
      *begin.get_component<int>(0)
  );
}



