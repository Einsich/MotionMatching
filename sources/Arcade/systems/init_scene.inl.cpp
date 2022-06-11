#include "init_scene.inl"
#include <ecs_perform.h>
//Code-generator production

void init_sprites_shaders_camera_handler(const ecs::Event &event);
void init_sprites_shaders_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_sprites_shaders_camera_descr("init_sprites_shaders_camera", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false}
}, {
}, {},
{},
{},
init_sprites_shaders_camera_handler, init_sprites_shaders_camera_singl_handler, ecs::tags::all);

void init_sprites_shaders_camera_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_sprites_shaders_camera_descr, init_sprites_shaders_camera);
}
void init_sprites_shaders_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_sprites_shaders_camera_descr, eid, init_sprites_shaders_camera);
}

void load_scene_event_handler(const ecs::Event &event);
void load_scene_event_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<LoadSceneEvent> load_scene_event_descr("load_scene_event", {
}, {
}, {},
{},
{},
load_scene_event_handler, load_scene_event_singl_handler, ecs::tags::all);

void load_scene_event_handler(const ecs::Event &event)
{
  ecs::perform_event((const LoadSceneEvent&)event, load_scene_event_descr, load_scene_event);
}
void load_scene_event_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const LoadSceneEvent&)event, load_scene_event_descr, eid, load_scene_event);
}


