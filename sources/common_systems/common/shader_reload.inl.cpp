#include "shader_reload.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache update_material__cache__;

static ecs::QueryCache reload_shaders__cache__;

static ecs::QueryCache load_directional_light__cache__;

static ecs::QueryCache reload_directional_light__cache__;

template<typename Callable>
static void update_material(Callable lambda)
{
  ecs::perform_query<Asset<Material>&>(update_material__cache__, lambda);
}

static void reload_shaders_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyDownEvent<SDLK_F5> &>(event), reload_shaders__cache__, reload_shaders);
}

static void reload_shaders_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyDownEvent<SDLK_F5> &>(event), reload_shaders__cache__, reload_shaders);
}

static void load_directional_light_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), load_directional_light__cache__, load_directional_light);
}

static void load_directional_light_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), load_directional_light__cache__, load_directional_light);
}

static void reload_directional_light_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const OnEntityEdited &>(event), reload_directional_light__cache__, reload_directional_light);
}

static void reload_directional_light_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const OnEntityEdited &>(event), reload_directional_light__cache__, reload_directional_light);
}

static void registration_pull_shader_reload()
{
  ecs::register_query(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/shader_reload.inl:29",
  "update_material",
  &update_material__cache__,
  {
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()}
  },
  {},
  {}
  );

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/shader_reload.inl:24",
  "reload_shaders",
  &reload_shaders__cache__,
  {},
  {},
  {},
  {},
  {},
  {"debug"},
  &reload_shaders_handler, &reload_shaders_single_handler,
  ecs::EventIndex<KeyDownEvent<SDLK_F5>>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/shader_reload.inl:33",
  "load_directional_light",
  &load_directional_light__cache__,
  {
    {"directionalLight", ecs::get_type_index<DirectionLight>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<DirectionLight>()}
  },
  {},
  {},
  {},
  {},
  {},
  &load_directional_light_handler, &load_directional_light_single_handler,
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/shader_reload.inl:38",
  "reload_directional_light",
  &reload_directional_light__cache__,
  {
    {"directionalLight", ecs::get_type_index<DirectionLight>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<DirectionLight>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &reload_directional_light_handler, &reload_directional_light_single_handler,
  ecs::EventIndex<OnEntityEdited>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_shader_reload)
ECS_PULL_DEFINITION(variable_pull_shader_reload)
