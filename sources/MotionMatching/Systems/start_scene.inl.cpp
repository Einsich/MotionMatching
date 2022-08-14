#include "start_scene.inl"
#include <ecs_perform.h>
//Code-generator production

void init_anim_settings_handler(const ecs::Event &event);
void init_anim_settings_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_anim_settings_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "init_anim_settings", {
  {ecs::get_type_hash<ecs::vector<AnimationTest>>(), ecs::get_name_hash("tests"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false},
  {ecs::get_type_hash<SettingsContainer>(), ecs::get_name_hash("settingsContainer"), false}
}, {
},
{},
{},
init_anim_settings_handler, init_anim_settings_singl_handler, {"game"});

void init_anim_settings_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, init_anim_settings_descr, init_anim_settings);
}
void init_anim_settings_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, init_anim_settings_descr, eid, init_anim_settings);
}

void scene_destroy_handler(const ecs::Event &event);
void scene_destroy_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription scene_destroy_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityDestroyed>(), "scene_destroy", {
  {ecs::get_type_hash<ecs::vector<AnimationTest>>(), ecs::get_name_hash("tests"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false},
  {ecs::get_type_hash<SettingsContainer>(), ecs::get_name_hash("settingsContainer"), false}
}, {
},
{},
{},
scene_destroy_handler, scene_destroy_singl_handler, {});

void scene_destroy_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityDestroyed&)event, scene_destroy_descr, scene_destroy);
}
void scene_destroy_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityDestroyed&)event, scene_destroy_descr, eid, scene_destroy);
}


