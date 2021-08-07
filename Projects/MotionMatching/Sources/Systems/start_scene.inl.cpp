#include "start_scene.inl"
//Code-generator production

void init_anim_settings_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_anim_settings_descr("init_anim_settings", {
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, init_anim_settings_handler, (uint)(ecs::SystemTag::Game));

void init_anim_settings_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = init_anim_settings_descr.begin(), end = init_anim_settings_descr.end(); begin != end; ++begin)
  {
    init_anim_settings(
      event,
      *begin.get_component<Settings>(0),
      *begin.get_component<SettingsContainer>(1)
    );
  }
}


void scene_destroy_handler(const ecs::OnEntityDestroyed &event);

ecs::EventDescription<ecs::OnEntityDestroyed> scene_destroy_descr("scene_destroy", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, scene_destroy_handler, (uint)(ecs::SystemTag::Game));

void scene_destroy_handler(const ecs::OnEntityDestroyed &event)
{
  for (ecs::QueryIterator begin = scene_destroy_descr.begin(), end = scene_destroy_descr.end(); begin != end; ++begin)
  {
    scene_destroy(
      event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<Settings>(1),
      *begin.get_component<SettingsContainer>(2)
    );
  }
}


void init_anim_settings_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_anim_settings_singl_descr("init_anim_settings", {
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, init_anim_settings_singl_handler, (uint)(ecs::SystemTag::Game));

void init_anim_settings_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  init_anim_settings(
    event,
      *begin.get_component<Settings>(0),
      *begin.get_component<SettingsContainer>(1)
  );
}


void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityDestroyed> scene_destroy_singl_descr("scene_destroy", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, scene_destroy_singl_handler, (uint)(ecs::SystemTag::Game));

void scene_destroy_singl_handler(const ecs::OnEntityDestroyed &event, ecs::QueryIterator &begin)
{
  scene_destroy(
    event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<Settings>(1),
      *begin.get_component<SettingsContainer>(2)
  );
}



