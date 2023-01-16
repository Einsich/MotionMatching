#include "init_scene.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache init_sprites_shaders_camera__cache__;

static ecs::QueryCache load_scene_event__cache__;

static void init_sprites_shaders_camera_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init_sprites_shaders_camera__cache__, init_sprites_shaders_camera);
}

static void init_sprites_shaders_camera_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init_sprites_shaders_camera__cache__, init_sprites_shaders_camera);
}

static void load_scene_event_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const LoadSceneEvent &>(event), load_scene_event__cache__, load_scene_event);
}

static void load_scene_event_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const LoadSceneEvent &>(event), load_scene_event__cache__, load_scene_event);
}

static void registration_pull_init_scene()
{
  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/init_scene.inl:8",
  "init_sprites_shaders_camera",
  &init_sprites_shaders_camera__cache__,
  {
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<WorldRenderer>()},
    {"sf", ecs::get_type_index<SpriteFactory>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SpriteFactory>()}
  },
  {},
  {},
  {},
  {},
  {},
  &init_sprites_shaders_camera_handler, &init_sprites_shaders_camera_single_handler,
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Arcade/systems/init_scene.inl:33",
  "load_scene_event",
  &load_scene_event__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &load_scene_event_handler, &load_scene_event_single_handler,
  ecs::EventIndex<LoadSceneEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_init_scene)
ECS_PULL_DEFINITION(variable_pull_init_scene)
