#include "terrain.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription query_water_descr("query_water", {
  {ecs::get_type_description<Asset<Texture2D>>("water_noise_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("water_color_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("water_foam_texture"), false},
  {ecs::get_type_description<Asset<CubeMap>>("sky_reflection"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("isWater"), false}
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
  {ecs::get_type_description<MapRenderData>("data"), false}
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
  {ecs::get_type_description<Asset<Texture2D>>("heights_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("normal_texture"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_diffuse_array"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_normal_array"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_colormap_array"), false},
  {ecs::get_type_description<Asset<Texture2D>>("terrain_types_texture"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<Asset<Material>>("physycal_material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::vector<float>>("lods_distances"), false},
  {ecs::get_type_description<ecs::vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<int>("terrain_lods_count"), false},
  {ecs::get_type_description<float>("first_lod_distance"), false},
  {ecs::get_type_description<ecs::string>("terrain_texture"), false},
  {ecs::get_type_description<ecs::string>("tree_map"), false},
  {ecs::get_type_description<float>("tree_scale"), false},
  {ecs::get_type_description<ecs::vector<ivec3>>("terrain_type_color"), false},
  {ecs::get_type_description<ecs::vector<int>>("terrain_type_index"), false},
  {ecs::get_type_description<float>("pixel_scale"), false},
  {ecs::get_type_description<int>("water_level"), false},
  {ecs::get_type_description<vec2>("map_size"), false},
  {ecs::get_type_description<HeightMap>("heigth_map"), false},
  {ecs::get_type_description<MapRenderData>("data"), false}
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


