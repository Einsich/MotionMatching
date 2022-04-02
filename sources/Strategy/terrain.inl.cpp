#include "terrain.inl"
//Code-generator production

void create_terrain_handler(const ecs::OnEntityCreated &event);
void create_terrain_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> create_terrain_descr("create_terrain", {
  {ecs::get_type_description<Asset<Texture2D>>("heights_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("provinces_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("normal_texture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("terrain_texture"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_diffuse_array"), false},
  {ecs::get_type_description<Asset<Texture2DArray>>("terrain_normal_array"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
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


