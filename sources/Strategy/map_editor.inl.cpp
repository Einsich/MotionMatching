#include "map_editor.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache toggle_water__cache__;

static ecs::QueryCache country_builder__cache__;

static ecs::QueryCache trace_province__cache__;

static ecs::QueryCache selecte__cache__;

template<typename Callable>
static void toggle_water(Callable lambda)
{
  ecs::perform_query<bool&>(toggle_water__cache__, lambda);
}

static void country_builder_implementation()
{
  ecs::perform_system(country_builder__cache__, country_builder);
}

static void trace_province_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseClickEvent &>(event), trace_province__cache__, trace_province);
}

static void trace_province_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseClickEvent &>(event), trace_province__cache__, trace_province);
}

static void selecte_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseClickEvent &>(event), selecte__cache__, selecte);
}

static void selecte_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseClickEvent &>(event), selecte__cache__, selecte);
}

static void registration_pull_map_editor()
{
  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/map_editor.inl:55",
  "toggle_water",
  &toggle_water__cache__,
  {
    {"is_enabled", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"isWater", ecs::TypeIndex<ecs::Tag>::value}
  },
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/map_editor.inl:27",
  "country_builder",
  &country_builder__cache__,
  {
    {"editor", ecs::get_type_index<MapEditor>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapEditor>()},
    {"politicalMap", ecs::get_type_index<PoliticalMap>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<PoliticalMap>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {"editor"},
  &country_builder_implementation));

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/map_editor.inl:107",
  "trace_province",
  &trace_province__cache__,
  {
    {"political_material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"editor", ecs::get_type_index<MapEditor>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MapEditor>()},
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()},
    {"heightMap", ecs::get_type_index<HeightMap>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<HeightMap>()},
    {"politicalMap", ecs::get_type_index<PoliticalMap>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PoliticalMap>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &trace_province_handler, &trace_province_single_handler),
  ecs::EventIndex<MouseClickEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Strategy/map_editor.inl:139",
  "selecte",
  &selecte__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()},
    {"heightMap", ecs::get_type_index<HeightMap>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<HeightMap>()},
    {"politicalMap", ecs::get_type_index<PoliticalMap>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<PoliticalMap>()},
    {"renderData", ecs::get_type_index<MapRenderData>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MapRenderData>()}
  },
  {},
  {},
  {},
  {},
  {},
  &selecte_handler, &selecte_single_handler),
  ecs::EventIndex<MouseClickEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_map_editor)
ECS_PULL_DEFINITION(variable_pull_map_editor)
