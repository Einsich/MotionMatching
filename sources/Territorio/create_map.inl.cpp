#include "create_map.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription spawn_player_query_descr("spawn_player_query", {
  {ecs::get_type_hash<MapArrays>(), ecs::get_name_hash("map_arrays"), false},
  {ecs::get_type_hash<ecs::vector<vec3>>(), ecs::get_name_hash("land_colors"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("mapWasChanged"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("needUpdateBorder"), false}
}, {
});

template<typename Callable>
void spawn_player_query(Callable lambda)
{
  ecs::perform_query<MapArrays&, ecs::vector<vec3>&, bool&, bool&>
  (spawn_player_query_descr, lambda);
}


void create_map_handler(const ecs::Event &event);
void create_map_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription create_map_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "create_map", {
  {ecs::get_type_hash<int>(), ecs::get_name_hash("width"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("height"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("mapTexture"), false},
  {ecs::get_type_hash<Asset<Texture2D>>(), ecs::get_name_hash("borderTexture"), false},
  {ecs::get_type_hash<Shader>(), ecs::get_name_hash("mapShader"), false},
  {ecs::get_type_hash<MapArrays>(), ecs::get_name_hash("map_arrays"), false},
  {ecs::get_type_hash<ecs::vector<vec3>>(), ecs::get_name_hash("land_colors"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("botsCount"), false}
}, {
},
{},
{},
create_map_handler, create_map_singl_handler, {});

void create_map_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, create_map_descr, create_map);
}
void create_map_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, create_map_descr, eid, create_map);
}


