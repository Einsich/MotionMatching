#include "provinces.inl"
//Code-generator production

ecs::QueryDescription toggle_map_mode_descr("toggle_map_mode", {
  {ecs::get_type_description<bool>("is_enabled"), false},
  {ecs::get_type_description<ecs::Tag>("isTree"), false}
}, {
});

template<typename Callable>
void toggle_map_mode(Callable lambda)
{
  ecs::perform_query<bool&>
  (toggle_map_mode_descr, lambda);
}


void change_terrain_mode_handler(const KeyDownEvent<SDLK_m> &event);
void change_terrain_mode_singl_handler(const KeyDownEvent<SDLK_m> &event, ecs::EntityId eid);

ecs::EventDescription<KeyDownEvent<SDLK_m>> change_terrain_mode_descr("change_terrain_mode", {
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<Asset<Material>>("physycal_material"), false}
}, {
}, {"game","editor"},
change_terrain_mode_handler, change_terrain_mode_singl_handler, ecs::tags::all);

void change_terrain_mode_handler(const KeyDownEvent<SDLK_m> &event)
{
  ecs::perform_event(event, change_terrain_mode_descr, change_terrain_mode);
}
void change_terrain_mode_singl_handler(const KeyDownEvent<SDLK_m> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, change_terrain_mode_descr, eid, change_terrain_mode);
}

void create_provinces_handler(const ecs::OnSceneCreated &event);
void create_provinces_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> create_provinces_descr("create_provinces", {
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<Asset<Texture2D>>("provinces_texture"), false},
  {ecs::get_type_description<string>("provinces_texture_name"), false},
  {ecs::get_type_description<string>("load_provinces_info"), false},
  {ecs::get_type_description<float>("pixel_scale"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false},
  {ecs::get_type_description<MapRenderData>("data"), false}
}, {
}, {"game","editor"},
create_provinces_handler, create_provinces_singl_handler, ecs::tags::all);

void create_provinces_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, create_provinces_descr, create_provinces);
}
void create_provinces_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, create_provinces_descr, eid, create_provinces);
}


