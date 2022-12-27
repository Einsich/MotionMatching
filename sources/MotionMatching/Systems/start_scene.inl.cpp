#include "start_scene.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache init_anim_settings__cache__;

static ecs::QueryCache scene_destroy__cache__;

static void init_anim_settings_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), init_anim_settings__cache__, init_anim_settings);
}

static void init_anim_settings_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), init_anim_settings__cache__, init_anim_settings);
}

static void scene_destroy_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityDestroyed &>(event), scene_destroy__cache__, scene_destroy);
}

static void scene_destroy_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityDestroyed &>(event), scene_destroy__cache__, scene_destroy);
}

static void registration_pull_start_scene()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "init_anim_settings",
  &init_anim_settings__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SettingsContainer>()}
  },
  {},
  {},
  {},
  {},
  {"game"},
  &init_anim_settings_handler, &init_anim_settings_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "scene_destroy",
  &scene_destroy__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<SettingsContainer>()}
  },
  {},
  {},
  {},
  {},
  {},
  &scene_destroy_handler, &scene_destroy_single_handler),
  ecs::EventIndex<ecs::OnEntityDestroyed>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_start_scene)
ECS_PULL_DEFINITION(variable_pull_start_scene)
