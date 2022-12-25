#include "land_logic.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache lands_economic__cache__;

static ecs::QueryCache gather_invaders__cache__;

static ecs::QueryCache gather_invaders2__cache__;

static ecs::QueryCache query_victim2__cache__;

static ecs::QueryCache query_neighbor__cache__;

static ecs::QueryCache query_victim__cache__;

static ecs::QueryCache update_bot_invasions__cache__;

static ecs::QueryCache map_update__cache__;

static ecs::QueryCache border_update__cache__;

static ecs::QueryCache game_started__cache__;

template<typename Callable>
static void lands_economic(Callable lambda)
{
  ecs::perform_query<int, uint&>(lands_economic__cache__, lambda);
}

template<typename Callable>
static void gather_invaders(Callable lambda)
{
  ecs::perform_query<ecs::vector<Invasion>&, uint, int&>(gather_invaders__cache__, lambda);
}

template<typename Callable>
static void gather_invaders2(Callable lambda)
{
  ecs::perform_query<uint, ecs::vector<ecs::EntityId>&, ecs::vector<uint>&>(gather_invaders2__cache__, lambda);
}

template<typename Callable>
static void query_victim2(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, uint, int>(query_victim2__cache__, lambda);
}

template<typename Callable>
static void query_neighbor(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<uint, ecs::EntityId, uint>(query_neighbor__cache__, eid, lambda);
}

template<typename Callable>
static void query_victim(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<uint&, uint, int&>(query_victim__cache__, eid, lambda);
}

static void update_bot_invasions_implementation()
{
  ecs::perform_system(update_bot_invasions__cache__, update_bot_invasions);
}

static void map_update_implementation()
{
  ecs::perform_system(map_update__cache__, map_update);
}

static void border_update_implementation()
{
  ecs::perform_system(border_update__cache__, border_update);
}

static void game_started_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const OnGameStarted &>(event), game_started__cache__, game_started);
}

static void game_started_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const OnGameStarted &>(event), game_started__cache__, game_started);
}

static void registration_pull_land_logic()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "lands_economic",
  &lands_economic__cache__,
  {
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<uint>()}
  },
  {
    {"isPlayableLand", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_invaders",
  &gather_invaders__cache__,
  {
    {"invasions", ecs::get_type_index<ecs::vector<Invasion>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<Invasion>>()},
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_invaders2",
  &gather_invaders2__cache__,
  {
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"neighbors", ecs::get_type_index<ecs::vector<ecs::EntityId>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<ecs::EntityId>>()},
    {"neighborsIdx", ecs::get_type_index<ecs::vector<uint>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<uint>>()}
  },
  {
    {"isPlayableLand", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "query_victim2",
  &query_victim2__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "query_neighbor",
  &query_neighbor__cache__,
  {
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "query_victim",
  &query_victim__cache__,
  {
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<uint>()},
    {"landIndex", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "update_bot_invasions",
  &update_bot_invasions__cache__,
  {
    {"invasionPeriod", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"invasionTime", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<float>()},
    {"invasions", ecs::get_type_index<ecs::vector<Invasion>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<Invasion>>()},
    {"neighbors", ecs::get_type_index<ecs::vector<ecs::EntityId>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<ecs::EntityId>>()},
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<uint>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &update_bot_invasions_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "map_update",
  &map_update__cache__,
  {
    {"forceFromCell", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"forceFromPeople", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"maxForceFromCell", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"updatePeriod", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"updateTime", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<float>()},
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapArrays>()},
    {"mapWasChanged", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()},
    {"needUpdateBorder", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()},
    {"gameStarted", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &map_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "border_update",
  &border_update__cache__,
  {
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapArrays>()},
    {"needUpdateBorder", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &border_update_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "game_started",
  &game_started__cache__,
  {
    {"forces", ecs::get_type_index<uint>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<uint>()},
    {"startForces", ecs::get_type_index<uint>(), ecs::AccessType::Copy, false, ecs::is_singleton<uint>()},
    {"landCount", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()}
  },
  {
    {"isPlayableLand", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {},
  {},
  {},
  &game_started_handler, &game_started_single_handler),
  ecs::EventIndex<OnGameStarted>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_land_logic)
