#include "start_scene.inl"
//Code-generator production

void init_anim_settings_handler(const ecs::OnSceneCreated &event);
void init_anim_settings_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_anim_settings_descr("init_anim_settings", {
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, init_anim_settings_handler, init_anim_settings_singl_handler, ecs::SystemTag::Game);

void init_anim_settings_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_anim_settings_descr, init_anim_settings);
}
void init_anim_settings_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_anim_settings_descr, eid, init_anim_settings);
}

void init_blk_scene_handler(const ecs::OnSceneCreated &event);
void init_blk_scene_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_blk_scene_descr("init_blk_scene", {
}, init_blk_scene_handler, init_blk_scene_singl_handler, ecs::SystemTag::GameEditor);

void init_blk_scene_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_blk_scene_descr, init_blk_scene);
}
void init_blk_scene_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_blk_scene_descr, eid, init_blk_scene);
}

void scene_destroy_handler(const ecs::OnEntityDestroyed &event);
void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityDestroyed> scene_destroy_descr("scene_destroy", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, scene_destroy_handler, scene_destroy_singl_handler, ecs::SystemTag::Game);

void scene_destroy_handler(const ecs::OnEntityDestroyed &event)
{
  ecs::perform_event(event, scene_destroy_descr, scene_destroy);
}
void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::EntityId eid)
{
  ecs::perform_event(event, scene_destroy_descr, eid, scene_destroy);
}


