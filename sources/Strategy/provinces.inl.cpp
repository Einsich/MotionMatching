#include "provinces.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache toggle_map_mode__cache__;

static ecs::QueryCache change_terrain_mode__cache__;

static ecs::QueryCache create_provinces__cache__;

template<typename Callable>
static void toggle_map_mode(Callable lambda)
{
  ecs::perform_query<bool&>(toggle_map_mode__cache__, lambda);
}

static void change_terrain_mode_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyDownEvent<SDLK_m> &>(event), change_terrain_mode__cache__, change_terrain_mode);
}

static void change_terrain_mode_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyDownEvent<SDLK_m> &>(event), change_terrain_mode__cache__, change_terrain_mode);
}

static void create_provinces_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), create_provinces__cache__, create_provinces);
}

static void create_provinces_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), create_provinces__cache__, create_provinces);
}

static void registration_pull_provinces()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "toggle_map_mode",
  &toggle_map_mode__cache__,
  {
    {"is_enabled", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"isTree", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_event(ecs::EventDescription(
  "",
  "change_terrain_mode",
  &change_terrain_mode__cache__,
  {
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"political_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Material>>()},
    {"physycal_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Material>>()}
  },
  {},
  {},
  {},
  {},
  {},
  &change_terrain_mode_handler, &change_terrain_mode_single_handler),
  ecs::EventIndex<KeyDownEvent<SDLK_m>>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "create_provinces",
  &create_provinces__cache__,
  {
    {"political_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"provinces_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"provinces_texture_name", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"load_provinces_info", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"pixel_scale", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"politicalMap", ecs::get_type_index<PoliticalMap>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PoliticalMap>()},
    {"data", ecs::get_type_index<MapRenderData>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapRenderData>()}
  },
  {},
  {},
  {},
  {},
  {},
  &create_provinces_handler, &create_provinces_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_provinces)
ECS_PULL_DEFINITION(variable_pull_provinces)
