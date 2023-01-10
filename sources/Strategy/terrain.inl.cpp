#include "terrain.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache query_water__cache__;

static ecs::QueryCache set_map_render_data__cache__;

static ecs::QueryCache add_map_uniform__cache__;

static ecs::QueryCache create_terrain__cache__;

template<typename Callable>
static void query_water(Callable lambda)
{
  ecs::perform_query<const Asset<Texture2D>&, const Asset<Texture2D>&, const Asset<Texture2D>&, const Asset<CubeMap>&, Asset<Material>&, Transform&>(query_water__cache__, lambda);
}

static void set_map_render_data_implementation()
{
  ecs::perform_system(set_map_render_data__cache__, set_map_render_data);
}

static void add_map_uniform_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), add_map_uniform__cache__, add_map_uniform);
}

static void add_map_uniform_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), add_map_uniform__cache__, add_map_uniform);
}

static void create_terrain_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), create_terrain__cache__, create_terrain);
}

static void create_terrain_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), create_terrain__cache__, create_terrain);
}

static void registration_pull_terrain()
{
  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/terrain.inl:247",
  "query_water",
  &query_water__cache__,
  {
    {"water_noise_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"water_color_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"water_foam_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"sky_reflection", ecs::get_type_index<Asset<CubeMap>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<CubeMap>>()},
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {
    {"isWater", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/terrain.inl:15",
  "set_map_render_data",
  &set_map_render_data__cache__,
  {
    {"data", ecs::get_type_index<MapRenderData>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MapRenderData>()}
  },
  {},
  {},
  "render",
  {},
  {},
  {},
  &set_map_render_data_implementation));

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/terrain.inl:11",
  "add_map_uniform",
  &add_map_uniform__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &add_map_uniform_handler, &add_map_uniform_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/terrain.inl:163",
  "create_terrain",
  &create_terrain__cache__,
  {
    {"heights_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"normal_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"terrain_diffuse_array", ecs::get_type_index<Asset<Texture2DArray>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2DArray>>()},
    {"terrain_normal_array", ecs::get_type_index<Asset<Texture2DArray>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2DArray>>()},
    {"terrain_colormap_array", ecs::get_type_index<Asset<Texture2DArray>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Texture2DArray>>()},
    {"terrain_types_texture", ecs::get_type_index<Asset<Texture2D>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Texture2D>>()},
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"political_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"physycal_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"lods_distances", ecs::get_type_index<ecs::vector<float>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<float>>()},
    {"lods_meshes", ecs::get_type_index<ecs::vector<Asset<Mesh>>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<Asset<Mesh>>>()},
    {"terrain_lods_count", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"first_lod_distance", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"terrain_texture", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"tree_map", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"tree_scale", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"terrain_type_color", ecs::get_type_index<ecs::vector<ivec3>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<ivec3>>()},
    {"terrain_type_index", ecs::get_type_index<ecs::vector<int>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<int>>()},
    {"pixel_scale", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"water_level", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"map_size", ecs::get_type_index<vec2>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec2>()},
    {"heigth_map", ecs::get_type_index<HeightMap>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<HeightMap>()},
    {"data", ecs::get_type_index<MapRenderData>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapRenderData>()}
  },
  {},
  {},
  {},
  {},
  {},
  &create_terrain_handler, &create_terrain_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_terrain)
ECS_PULL_DEFINITION(variable_pull_terrain)
