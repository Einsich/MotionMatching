#include "animation_player.inl"
//Code-generator production

void animation_player_update_func();

ecs::SystemDescription animation_player_update_descr("animation_player_update", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<int>("mmIndex"), true},
  {ecs::get_type_description<int>("mmOptimisationIndex"), true},
  {ecs::get_type_description<bool>("updateMMStatistic"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, animation_player_update_func, ecs::SystemOrder::LOGIC, ecs::SystemTag::Game);

void animation_player_update_func()
{
  ecs::perform_system(animation_player_update_descr, animation_player_update);
}

void init_animation_character_handler(const ecs::OnEntityCreated &event);
void init_animation_character_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> init_animation_character_descr("init_animation_character", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("lods_meshes"), true},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, init_animation_character_handler, init_animation_character_singl_handler, ecs::SystemTag::GameEditor);

void init_animation_character_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, init_animation_character_descr, init_animation_character);
}
void init_animation_character_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_animation_character_descr, eid, init_animation_character);
}

void init_animation_material_handler(const ecs::OnEntityCreated &event);
void init_animation_material_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> init_animation_material_descr("init_animation_material", {
  {ecs::get_type_description<Asset<Material>>("material"), false}
}, init_animation_material_handler, init_animation_material_singl_handler, ecs::SystemTag::Game);

void init_animation_material_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, init_animation_material_descr, init_animation_material);
}
void init_animation_material_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_animation_material_descr, eid, init_animation_material);
}


