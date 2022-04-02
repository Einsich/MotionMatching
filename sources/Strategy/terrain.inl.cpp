#include "terrain.inl"
//Code-generator production

void terrain_lod_selector_func();

ecs::SystemDescription terrain_lod_selector_descr("terrain_lod_selector", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("terrain_lods_meshes"), false},
  {ecs::get_type_description<vector<float>>("lods_distances"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
}, terrain_lod_selector_func, ecs::SystemOrder::RENDER-1, ecs::SystemTag::GameEditor,
{},
{});

void terrain_lod_selector_func()
{
  ecs::perform_system(terrain_lod_selector_descr, terrain_lod_selector);
}

void create_terrain_handler(const ecs::OnEntityCreated &event);
void create_terrain_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> create_terrain_descr("create_terrain", {
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
  {ecs::get_type_description<vector<Asset<Mesh>>>("terrain_lods_meshes"), false},
  {ecs::get_type_description<int>("terrain_lods_count"), false},
  {ecs::get_type_description<float>("first_lod_distance"), false},
  {ecs::get_type_description<string>("terrain_texture"), false},
  {ecs::get_type_description<vector<ivec3>>("terrain_type_color"), false},
  {ecs::get_type_description<vector<int>>("terrain_type_index"), false},
  {ecs::get_type_description<float>("pixel_scale"), false}
}, create_terrain_handler, create_terrain_singl_handler, ecs::SystemTag::GameEditor);

void create_terrain_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, create_terrain_descr, create_terrain);
}
void create_terrain_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, create_terrain_descr, eid, create_terrain);
}


