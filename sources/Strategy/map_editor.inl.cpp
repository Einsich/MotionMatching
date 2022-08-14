#include "map_editor.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription toggle_water_descr("toggle_water", {
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("is_enabled"), false},
  {-1u, ecs::get_name_hash("isWater"), false}
}, {
});

template<typename Callable>
void toggle_water(Callable lambda)
{
  ecs::perform_query<bool&>
  (toggle_water_descr, lambda);
}


void country_builder_func();

ecs::SystemDescription country_builder_descr("country_builder", {
  {ecs::get_type_hash<MapEditor>(), ecs::get_name_hash("editor"), false},
  {ecs::get_type_hash<PoliticalMap>(), ecs::get_name_hash("politicalMap"), false}
}, {
},
{},
{},
country_builder_func, "ui", {"editor"}, false);

void country_builder_func()
{
  ecs::perform_system(country_builder_descr, country_builder);
}

void trace_province_handler(const ecs::Event &event);
void trace_province_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription trace_province_descr(
  ecs::get_mutable_event_handlers<MouseClickEvent>(), "trace_province", {
  {ecs::get_type_hash<Asset<Material>>(), ecs::get_name_hash("political_material"), false},
  {ecs::get_type_hash<MapEditor>(), ecs::get_name_hash("editor"), false},
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false},
  {ecs::get_type_hash<HeightMap>(), ecs::get_name_hash("heightMap"), false},
  {ecs::get_type_hash<PoliticalMap>(), ecs::get_name_hash("politicalMap"), false}
}, {
},
{},
{},
trace_province_handler, trace_province_singl_handler, {"editor"});

void trace_province_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseClickEvent&)event, trace_province_descr, trace_province);
}
void trace_province_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseClickEvent&)event, trace_province_descr, eid, trace_province);
}

void selecte_handler(const ecs::Event &event);
void selecte_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription selecte_descr(
  ecs::get_mutable_event_handlers<MouseClickEvent>(), "selecte", {
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false},
  {ecs::get_type_hash<HeightMap>(), ecs::get_name_hash("heightMap"), false},
  {ecs::get_type_hash<PoliticalMap>(), ecs::get_name_hash("politicalMap"), false},
  {ecs::get_type_hash<MapRenderData>(), ecs::get_name_hash("renderData"), false}
}, {
},
{},
{},
selecte_handler, selecte_singl_handler, {});

void selecte_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseClickEvent&)event, selecte_descr, selecte);
}
void selecte_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseClickEvent&)event, selecte_descr, eid, selecte);
}


