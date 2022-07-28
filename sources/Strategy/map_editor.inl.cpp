#include "map_editor.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription toggle_water_descr("toggle_water", {
  {ecs::get_type_description<bool>("is_enabled"), false},
  {ecs::get_type_description<ecs::Tag>("isWater"), false}
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
  {ecs::get_type_description<MapEditor>("editor"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false}
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
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<MapEditor>("editor"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false}
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
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false},
  {ecs::get_type_description<MapRenderData>("renderData"), false}
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


