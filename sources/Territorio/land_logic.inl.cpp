#include "land_logic.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription lands_economic_descr("lands_economic", {
  {ecs::get_type_description<int>("landCount"), false},
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayableLand"), false}
}, {
});

template<typename Callable>
void lands_economic(Callable lambda)
{
  ecs::perform_query<int, uint&>
  (lands_economic_descr, lambda);
}


ecs::QueryDescription gather_invaders_descr("gather_invaders", {
  {ecs::get_type_description<vector<Invasion>>("invasions"), false},
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<int>("landCount"), false}
}, {
});

template<typename Callable>
void gather_invaders(Callable lambda)
{
  ecs::perform_query<vector<Invasion>&, uint, int&>
  (gather_invaders_descr, lambda);
}


ecs::QueryDescription gather_invaders2_descr("gather_invaders2", {
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<vector<ecs::EntityId>>("neighbors"), false},
  {ecs::get_type_description<vector<uint>>("neighborsIdx"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayableLand"), false}
}, {
});

template<typename Callable>
void gather_invaders2(Callable lambda)
{
  ecs::perform_query<uint, vector<ecs::EntityId>&, vector<uint>&>
  (gather_invaders2_descr, lambda);
}


ecs::QueryDescription query_victim2_descr("query_victim2", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<int>("landCount"), false}
}, {
});

template<typename Callable>
void query_victim2(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, uint, int>
  (query_victim2_descr, lambda);
}


ecs::QueryDescription query_neighbor_descr("query_neighbor", {
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<uint>("landIndex"), false}
}, {
});

template<typename Callable>
void query_neighbor(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<uint, ecs::EntityId, uint>
  (query_neighbor_descr, eid, lambda);
}


ecs::QueryDescription query_victim_descr("query_victim", {
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<int>("landCount"), false}
}, {
});

template<typename Callable>
void query_victim(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<uint&, uint, int&>
  (query_victim_descr, eid, lambda);
}


void update_bot_invasions_func();

ecs::SystemDescription update_bot_invasions_descr("update_bot_invasions", {
  {ecs::get_type_description<float>("invasionPeriod"), false},
  {ecs::get_type_description<float>("invasionTime"), false},
  {ecs::get_type_description<vector<Invasion>>("invasions"), false},
  {ecs::get_type_description<vector<ecs::EntityId>>("neighbors"), false},
  {ecs::get_type_description<uint>("forces"), false}
}, {
}, {},
{},
{},
update_bot_invasions_func, ecs::stage::act, ecs::tags::all, false);

void update_bot_invasions_func()
{
  ecs::perform_system(update_bot_invasions_descr, update_bot_invasions);
}

void map_update_func();

ecs::SystemDescription map_update_descr("map_update", {
  {ecs::get_type_description<float>("forceFromCell"), false},
  {ecs::get_type_description<float>("forceFromPeople"), false},
  {ecs::get_type_description<float>("maxForceFromCell"), false},
  {ecs::get_type_description<float>("updatePeriod"), false},
  {ecs::get_type_description<float>("updateTime"), false},
  {ecs::get_type_description<MapArrays>("map_arrays"), false},
  {ecs::get_type_description<bool>("mapWasChanged"), false},
  {ecs::get_type_description<bool>("needUpdateBorder"), false},
  {ecs::get_type_description<bool>("gameStarted"), false}
}, {
}, {},
{},
{},
map_update_func, ecs::stage::act, ecs::tags::all, false);

void map_update_func()
{
  ecs::perform_system(map_update_descr, map_update);
}

void border_update_func();

ecs::SystemDescription border_update_descr("border_update", {
  {ecs::get_type_description<MapArrays>("map_arrays"), false},
  {ecs::get_type_description<bool>("needUpdateBorder"), false}
}, {
}, {},
{},
{},
border_update_func, ecs::stage::act, ecs::tags::all, false);

void border_update_func()
{
  ecs::perform_system(border_update_descr, border_update);
}

void game_started_handler(const ecs::Event &event);
void game_started_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<OnGameStarted> game_started_descr("game_started", {
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<uint>("startForces"), false},
  {ecs::get_type_description<int>("landCount"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayableLand"), false}
}, {
}, {},
{},
{},
game_started_handler, game_started_singl_handler, ecs::tags::all);

void game_started_handler(const ecs::Event &event)
{
  ecs::perform_event((const OnGameStarted&)event, game_started_descr, game_started);
}
void game_started_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const OnGameStarted&)event, game_started_descr, eid, game_started);
}


