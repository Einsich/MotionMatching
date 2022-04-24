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
}, {"editor"},
{},
{},
country_builder_func, ecs::stage::ui, ecs::tags::all, false);

void country_builder_func()
{
  ecs::perform_system(country_builder_descr, country_builder);
}

void trace_province_handler(const MouseClickEvent &event);
void trace_province_singl_handler(const MouseClickEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseClickEvent> trace_province_descr("trace_province", {
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<MapEditor>("editor"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false}
}, {
}, {"editor"},
{},
{},
trace_province_handler, trace_province_singl_handler, ecs::tags::all);

void trace_province_handler(const MouseClickEvent &event)
{
  ecs::perform_event(event, trace_province_descr, trace_province);
}
void trace_province_singl_handler(const MouseClickEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, trace_province_descr, eid, trace_province);
}

void selecte_handler(const MouseClickEvent &event);
void selecte_singl_handler(const MouseClickEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseClickEvent> selecte_descr("selecte", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false},
  {ecs::get_type_description<MapRenderData>("renderData"), false}
}, {
}, {"game","editor"},
{},
{},
selecte_handler, selecte_singl_handler, ecs::tags::all);

void selecte_handler(const MouseClickEvent &event)
{
  ecs::perform_event(event, selecte_descr, selecte);
}
void selecte_singl_handler(const MouseClickEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, selecte_descr, eid, selecte);
}


