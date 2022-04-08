#include "country_builder.inl"
//Code-generator production

ecs::QueryDescription toggle_water_descr("toggle_water", {
  {ecs::get_type_description<bool>("is_enabled"), false},
  {ecs::get_type_description<ecs::Tag>("isWater"), false}
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
}, country_builder_func, ecs::SystemOrder::UI,  ecs::SystemTag::Editor,
{},
{});

void country_builder_func()
{
  ecs::perform_system(country_builder_descr, country_builder);
}

void trace_province_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);
void trace_province_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> trace_province_descr("trace_province", {
  {ecs::get_type_description<Asset<Material>>("political_material"), false},
  {ecs::get_type_description<MapEditor>("editor"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<HeightMap>("heightMap"), false},
  {ecs::get_type_description<PoliticalMap>("politicalMap"), false}
}, trace_province_handler, trace_province_singl_handler, ecs::SystemTag::Editor);

void trace_province_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  ecs::perform_event(event, trace_province_descr, trace_province);
}
void trace_province_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, trace_province_descr, eid, trace_province);
}


