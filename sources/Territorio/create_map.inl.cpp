#include "create_map.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache spawn_player_query__cache__;

static ecs::QueryCache create_map__cache__;

template<typename Callable>
static void spawn_player_query(Callable lambda)
{
  ecs::perform_query<MapArrays&, ecs::vector<vec3>&, bool&, bool&>(spawn_player_query__cache__, lambda);
}

static void create_map_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), create_map__cache__, create_map);
}

static void create_map_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), create_map__cache__, create_map);
}

static void registration_pull_create_map()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "spawn_player_query",
  &spawn_player_query__cache__,
  {
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapArrays>()},
    {"land_colors", ecs::get_type_index<ecs::vector<vec3>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec3>>()},
    {"mapWasChanged", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()},
    {"needUpdateBorder", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {},
  {}
  ));

  ecs::register_event(ecs::EventDescription(
  "",
  "create_map",
  &create_map__cache__,
  {
    {"width", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"height", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"mapTexture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"borderTexture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"mapShader", ecs::get_type_index<Shader>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Shader>()},
    {"map_arrays", ecs::get_type_index<MapArrays>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapArrays>()},
    {"land_colors", ecs::get_type_index<ecs::vector<vec3>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec3>>()},
    {"botsCount", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()}
  },
  {},
  {},
  {},
  {},
  {},
  &create_map_handler, &create_map_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_create_map)
ECS_PULL_DEFINITION(variable_pull_create_map)
