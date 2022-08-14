#include "provinces.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription toggle_map_mode_descr("toggle_map_mode", {
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("is_enabled"), false},
  {-1u, ecs::get_name_hash("isTree"), false}
}, {
});

template<typename Callable>
void toggle_map_mode(Callable lambda)
{
  ecs::perform_query<bool&>
  (toggle_map_mode_descr, lambda);
}


void change_terrain_mode_handler(const ecs::Event &event);
void change_terrain_mode_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription change_terrain_mode_descr(
  ecs::get_mutable_event_handlers<KeyDownEvent<SDLK_m>>(), "change_terrain_mode", {
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("material"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("political_material"), false},
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("physycal_material"), false}
}, {
},
{},
{},
change_terrain_mode_handler, change_terrain_mode_singl_handler, {});

void change_terrain_mode_handler(const ecs::Event &event)
{
  ecs::perform_event((const KeyDownEvent<SDLK_m>&)event, change_terrain_mode_descr, change_terrain_mode);
}
void change_terrain_mode_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const KeyDownEvent<SDLK_m>&)event, change_terrain_mode_descr, eid, change_terrain_mode);
}

void create_provinces_handler(const ecs::Event &event);
void create_provinces_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription create_provinces_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "create_provinces", {
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("political_material"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("provinces_texture"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("provinces_texture_name"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("load_provinces_info"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("pixel_scale"), false},
  {ecs::get_type_hash<PoliticalMap>(), ecs::get_name_hash("politicalMap"), false},
  {ecs::get_type_hash<MapRenderData>(), ecs::get_name_hash("data"), false}
}, {
},
{},
{},
create_provinces_handler, create_provinces_singl_handler, {});

void create_provinces_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, create_provinces_descr, create_provinces);
}
void create_provinces_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, create_provinces_descr, eid, create_provinces);
}


