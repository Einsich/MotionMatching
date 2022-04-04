#include "terrain.inl"
//Code-generator production

ecs::QueryDescription query_water_descr("query_water", {
  {ecs::get_type_description<Asset<Texture2D>>("water_noise_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("water_color_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("water_foam_texture"), false},
  {ecs::get_type_description<Asset<CubeMap>>("sky_reflection"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("isWater"), false}
});

template<typename Callable>
void query_water(Callable lambda)
{
  ecs::perform_query<Asset<Texture2D>&, Asset<Texture2D>&, Asset<Texture2D>&, Asset<CubeMap>&, Asset<Material>&, Transform&>
  (query_water_descr, lambda);
}


void create_terrain_handler(const ecs::OnSceneCreated &event);
void create_terrain_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> create_terrain_descr("create_terrain", {
  {ecs::get_type_description<Asset<Texture2D>>("heights_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("provinces_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("normal_texture"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_diffuse_array"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_normal_array"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_colormap_array"), false},
  {ecs::get_type_description<Asset<Texture2D>>("terrain_types_texture"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<vector<float>>("lods_distances"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<int>("terrain_lods_count"), false},
  {ecs::get_type_description<float>("first_lod_distance"), false},
  {ecs::get_type_description<string>("terrain_texture"), false},
  {ecs::get_type_description<string>("tree_map"), false},
  {ecs::get_type_description<float>("tree_scale"), false},
  {ecs::get_type_description<vector<ivec3>>("terrain_type_color"), false},
  {ecs::get_type_description<vector<int>>("terrain_type_index"), false},
  {ecs::get_type_description<float>("pixel_scale"), false},
  {ecs::get_type_description<int>("water_level"), false}
}, create_terrain_handler, create_terrain_singl_handler, ecs::SystemTag::GameEditor);

void create_terrain_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, create_terrain_descr, create_terrain);
}
void create_terrain_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, create_terrain_descr, eid, create_terrain);
}


