#include "land_logic.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription lands_economic_descr("lands_economic", {
  {ecs::get_type_hash<int>(), ecs::get_name_hash("landCount"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("forces"), false},
  {-1u, ecs::get_name_hash("isPlayableLand"), false}
}, {
});

template<typename Callable>
void lands_economic(Callable lambda)
{
  ecs::perform_query<int, uint&>
  (lands_economic_descr, lambda);
}


ecs::QueryDescription gather_invaders_descr("gather_invaders", {
  {ecs::get_type_hash<ecs::vector<Invasion>>(), ecs::get_name_hash("invasions"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("landIndex"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("landCount"), false}
}, {
});

template<typename Callable>
void gather_invaders(Callable lambda)
{
  ecs::perform_query<ecs::vector<Invasion>&, uint, int&>
  (gather_invaders_descr, lambda);
}


ecs::QueryDescription gather_invaders2_descr("gather_invaders2", {
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("landIndex"), false},
  {ecs::get_type_hash<ecs::vector<ecs::EntityId>>(), ecs::get_name_hash("neighbors"), false},
  {ecs::get_type_hash<ecs::vector<uint>>(), ecs::get_name_hash("neighborsIdx"), false},
  {-1u, ecs::get_name_hash("isPlayableLand"), false}
}, {
});

template<typename Callable>
void gather_invaders2(Callable lambda)
{
  ecs::perform_query<uint, ecs::vector<ecs::EntityId>&, ecs::vector<uint>&>
  (gather_invaders2_descr, lambda);
}


ecs::QueryDescription query_victim2_descr("query_victim2", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("landIndex"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("landCount"), false}
}, {
});

template<typename Callable>
void query_victim2(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, uint, int>
  (query_victim2_descr, lambda);
}


ecs::QueryDescription query_neighbor_descr("query_neighbor", {
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("forces"), false},
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("landIndex"), false}
}, {
});

template<typename Callable>
void query_neighbor(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<uint, ecs::EntityId, uint>
  (query_neighbor_descr, eid, lambda);
}


ecs::QueryDescription query_victim_descr("query_victim", {
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("forces"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("landIndex"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("landCount"), false}
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
  {ecs::get_type_hash<float>(), ecs::get_name_hash("invasionPeriod"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("invasionTime"), false},
  {ecs::get_type_hash<ecs::vector<Invasion>>(), ecs::get_name_hash("invasions"), false},
  {ecs::get_type_hash<ecs::vector<ecs::EntityId>>(), ecs::get_name_hash("neighbors"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("forces"), false}
}, {
},
{},
{},
update_bot_invasions_func, "act", {}, false);

void update_bot_invasions_func()
{
  ecs::perform_system(update_bot_invasions_descr, update_bot_invasions);
}

void map_update_func();

ecs::SystemDescription map_update_descr("map_update", {
  {ecs::get_type_hash<float>(), ecs::get_name_hash("forceFromCell"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("forceFromPeople"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("maxForceFromCell"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("updatePeriod"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("updateTime"), false},
  {ecs::get_type_hash<MapArrays>(), ecs::get_name_hash("map_arrays"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("mapWasChanged"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("needUpdateBorder"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("gameStarted"), false}
}, {
},
{},
{},
map_update_func, "act", {}, false);

void map_update_func()
{
  ecs::perform_system(map_update_descr, map_update);
}

void border_update_func();

ecs::SystemDescription border_update_descr("border_update", {
  {ecs::get_type_hash<MapArrays>(), ecs::get_name_hash("map_arrays"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("needUpdateBorder"), false}
}, {
},
{},
{},
border_update_func, "act", {}, false);

void border_update_func()
{
  ecs::perform_system(border_update_descr, border_update);
}

void game_started_handler(const ecs::Event &event);
void game_started_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription game_started_descr(
  ecs::get_mutable_event_handlers<OnGameStarted>(), "game_started", {
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("forces"), false},
  {ecs::get_type_hash<uint>(), ecs::get_name_hash("startForces"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("landCount"), false},
  {-1u, ecs::get_name_hash("isPlayableLand"), false}
}, {
},
{},
{},
game_started_handler, game_started_singl_handler, {});

void game_started_handler(const ecs::Event &event)
{
  ecs::perform_event((const OnGameStarted&)event, game_started_descr, game_started);
}
void game_started_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const OnGameStarted&)event, game_started_descr, eid, game_started);
}


