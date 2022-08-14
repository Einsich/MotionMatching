#include "terrain.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription query_water_descr("query_water", {
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("water_noise_texture"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("water_color_texture"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("water_foam_texture"), false},
  {ecs::get_type_hash<Asset<CubeMap>>(), ecs::get_name_hash("sky_reflection"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("material"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {-1u, ecs::get_name_hash("isWater"), false}
}, {
});

template<typename Callable>
void query_water(Callable lambda)
{
  ecs::perform_query<Asset<Texture2D>&, Asset<Texture2D>&, Asset<Texture2D>&, Asset<CubeMap>&, Asset<Material>&, Transform&>
  (query_water_descr, lambda);
}


void set_map_render_data_func();

ecs::SystemDescription set_map_render_data_descr("set_map_render_data", {
  {ecs::get_type_hash<MapRenderData>(), ecs::get_name_hash("data"), false}
}, {
},
{},
{},
set_map_render_data_func, "render", {}, false);

void set_map_render_data_func()
{
  ecs::perform_system(set_map_render_data_descr, set_map_render_data);
}

void add_map_uniform_handler(const ecs::Event &event);
void add_map_uniform_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription add_map_uniform_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "add_map_uniform", {
}, {
},
{},
{},
add_map_uniform_handler, add_map_uniform_singl_handler, {});

void add_map_uniform_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, add_map_uniform_descr, add_map_uniform);
}
void add_map_uniform_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, add_map_uniform_descr, eid, add_map_uniform);
}

void create_terrain_handler(const ecs::Event &event);
void create_terrain_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription create_terrain_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "create_terrain", {
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("heights_texture"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("normal_texture"), false},
  {ecs::get_type_hash<Asset<Texture2DArray>>(), ecs::get_name_hash("terrain_diffuse_array"), false},
  {ecs::get_type_hash<Asset<Texture2DArray>>(), ecs::get_name_hash("terrain_normal_array"), false},
  {ecs::get_type_hash<Asset<Texture2DArray>>(), ecs::get_name_hash("terrain_colormap_array"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("terrain_types_texture"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("material"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("political_material"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("physycal_material"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<ecs::vector<float>>(), ecs::get_name_hash("lods_distances"), false},
  {ecs::get_type_hash<ecs::vector<Asset<Mesh>>>(), ecs::get_name_hash("lods_meshes"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("terrain_lods_count"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("first_lod_distance"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("terrain_texture"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("tree_map"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("tree_scale"), false},
  {ecs::get_type_hash<ecs::vector<ivec3>>(), ecs::get_name_hash("terrain_type_color"), false},
  {ecs::get_type_hash<ecs::vector<int>>(), ecs::get_name_hash("terrain_type_index"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("pixel_scale"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("water_level"), false},
  {ecs::get_type_hash<vec2>(), ecs::get_name_hash("map_size"), false},
  {ecs::get_type_hash<HeightMap>(), ecs::get_name_hash("heigth_map"), false},
  {ecs::get_type_hash<MapRenderData>(), ecs::get_name_hash("data"), false}
}, {
},
{},
{},
create_terrain_handler, create_terrain_singl_handler, {});

void create_terrain_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, create_terrain_descr, create_terrain);
}
void create_terrain_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, create_terrain_descr, eid, create_terrain);
}


