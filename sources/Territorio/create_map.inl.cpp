#include "create_map.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription spawn_player_query_descr("spawn_player_query", {
  {ecs::get_type_description<MapArrays>("map_arrays"), false},
  {ecs::get_type_description<vector<vec3>>("land_colors"), false},
  {ecs::get_type_description<bool>("mapWasChanged"), false},
  {ecs::get_type_description<bool>("needUpdateBorder"), false}
}, {
});

template<typename Callable>
void spawn_player_query(Callable lambda)
{
  ecs::perform_query<MapArrays&, vector<vec3>&, bool&, bool&>
  (spawn_player_query_descr, lambda);
}


void create_map_handler(const ecs::Event &event);
void create_map_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> create_map_descr("create_map", {
  {ecs::get_type_description<int>("width"), false},
  {ecs::get_type_description<int>("height"), false},
  {ecs::get_type_description<Asset<Texture2D>>("mapTexture"), false},
  {ecs::get_type_description<Asset<Texture2D>>("borderTexture"), false},
  {ecs::get_type_description<Shader>("mapShader"), false},
  {ecs::get_type_description<MapArrays>("map_arrays"), false},
  {ecs::get_type_description<vector<vec3>>("land_colors"), false},
  {ecs::get_type_description<int>("botsCount"), false}
}, {
}, {},
{},
{},
create_map_handler, create_map_singl_handler, ecs::tags::all);

void create_map_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, create_map_descr, create_map);
}
void create_map_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, create_map_descr, eid, create_map);
}


