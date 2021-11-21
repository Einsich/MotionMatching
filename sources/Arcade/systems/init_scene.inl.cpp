#include "init_scene.inl"
//Code-generator production

void init_sprites_shaders_camera_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_sprites_shaders_camera_descr("init_sprites_shaders_camera", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, init_sprites_shaders_camera_handler, ecs::SystemTag::Game);

void init_sprites_shaders_camera_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_sprites_shaders_camera_descr, init_sprites_shaders_camera);
}


void init_sprites_shaders_camera_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_sprites_shaders_camera_singl_descr("init_sprites_shaders_camera", {
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<SpriteFactory>("sf"), false},
  {ecs::get_type_description<ScoreBoard>("sb"), false}
}, init_sprites_shaders_camera_singl_handler, ecs::SystemTag::Game);

void init_sprites_shaders_camera_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  init_sprites_shaders_camera(
    event,
      *begin.get_component<WorldRenderer, 0>(),
      *begin.get_component<SpriteFactory, 1>(),
      *begin.get_component<ScoreBoard, 2>()
  );
}



