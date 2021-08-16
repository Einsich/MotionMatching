#include "animation_player.inl"
//Code-generator production

void animation_player_update_func();

ecs::SystemDescription animation_player_update_descr("animation_player_update", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), true},
  {ecs::get_type_description<int>("mmIndex"), true},
  {ecs::get_type_description<int>("mmOptimisationIndex"), true},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, animation_player_update_func, ecs::SystemOrder::LOGIC, (uint)(ecs::SystemTag::Game));

void animation_player_update_func()
{
  for (ecs::QueryIterator begin = animation_player_update_descr.begin(), end = animation_player_update_descr.end(); begin != end; ++begin)
  {
    animation_player_update(
      *begin.get_component<Transform>(0),
      *begin.get_component<AnimationPlayer>(1),
      *begin.get_component<Asset<Material>>(2),
       begin.get_component<ThirdPersonController>(3),
       begin.get_component<int>(4),
       begin.get_component<int>(5),
      *begin.get_component<Settings>(6),
      *begin.get_component<SettingsContainer>(7),
      *begin.get_component<MainCamera>(8)
    );
  }
}


void init_animation_character_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> init_animation_character_descr("init_animation_character", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, init_animation_character_handler, (uint)(ecs::SystemTag::GameEditor));

void init_animation_character_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = init_animation_character_descr.begin(), end = init_animation_character_descr.end(); begin != end; ++begin)
  {
    init_animation_character(
      event,
      *begin.get_component<AnimationPlayer>(0)
    );
  }
}


void init_animation_material_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> init_animation_material_descr("init_animation_material", {
  {ecs::get_type_description<Asset<Material>>("material"), false}
}, init_animation_material_handler, (uint)(ecs::SystemTag::Game));

void init_animation_material_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = init_animation_material_descr.begin(), end = init_animation_material_descr.end(); begin != end; ++begin)
  {
    init_animation_material(
      event,
      *begin.get_component<Asset<Material>>(0)
    );
  }
}


void init_animation_character_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> init_animation_character_singl_descr("init_animation_character", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, init_animation_character_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void init_animation_character_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  init_animation_character(
    event,
      *begin.get_component<AnimationPlayer>(0)
  );
}


void init_animation_material_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> init_animation_material_singl_descr("init_animation_material", {
  {ecs::get_type_description<Asset<Material>>("material"), false}
}, init_animation_material_singl_handler, (uint)(ecs::SystemTag::Game));

void init_animation_material_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  init_animation_material(
    event,
      *begin.get_component<Asset<Material>>(0)
  );
}



