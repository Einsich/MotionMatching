#include "start_scene.inl"
#include <ecs_perform.h>
//Code-generator production

void init_anim_settings_handler(const ecs::OnEntityCreated &event);
void init_anim_settings_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> init_anim_settings_descr("init_anim_settings", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, {
}, {"game"},
{},
{},
init_anim_settings_handler, init_anim_settings_singl_handler, ecs::tags::all);

void init_anim_settings_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, init_anim_settings_descr, init_anim_settings);
}
void init_anim_settings_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_anim_settings_descr, eid, init_anim_settings);
}

void scene_destroy_handler(const ecs::OnEntityDestroyed &event);
void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityDestroyed> scene_destroy_descr("scene_destroy", {
  {ecs::get_type_description<vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, {
}, {},
{},
{},
scene_destroy_handler, scene_destroy_singl_handler, ecs::tags::all);

void scene_destroy_handler(const ecs::OnEntityDestroyed &event)
{
  ecs::perform_event(event, scene_destroy_descr, scene_destroy);
}
void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::EntityId eid)
{
  ecs::perform_event(event, scene_destroy_descr, eid, scene_destroy);
}


