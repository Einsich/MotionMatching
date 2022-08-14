#include "init_scene.inl"
#include <ecs_perform.h>
//Code-generator production

void init_sprites_shaders_camera_handler(const ecs::Event &event);
void init_sprites_shaders_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_sprites_shaders_camera_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "init_sprites_shaders_camera", {
  {ecs::get_type_hash<WorldRenderer>(), ecs::get_name_hash("wr"), false},
  {ecs::get_type_hash<SpriteFactory>(), ecs::get_name_hash("sf"), false}
}, {
},
{},
{},
init_sprites_shaders_camera_handler, init_sprites_shaders_camera_singl_handler, {});

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

ecs::EventDescription load_scene_event_descr(
  ecs::get_mutable_event_handlers<LoadSceneEvent>(), "load_scene_event", {
}, {
},
{},
{},
load_scene_event_handler, load_scene_event_singl_handler, {});

void load_scene_event_handler(const ecs::Event &event)
{
  ecs::perform_event((const LoadSceneEvent&)event, load_scene_event_descr, load_scene_event);
}
void load_scene_event_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const LoadSceneEvent&)event, load_scene_event_descr, eid, load_scene_event);
}


