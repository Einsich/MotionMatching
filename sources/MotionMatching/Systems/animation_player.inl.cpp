#include "animation_player.inl"
#include <ecs_perform.h>
//Code-generator production

void animation_player_update_func();

ecs::SystemDescription animation_player_update_descr("animation_player_update", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<int>("mmIndex"), true},
  {ecs::get_type_description<int>("mmOptimisationIndex"), true},
  {ecs::get_type_description<bool>("updateMMStatistic"), false},
  {ecs::get_type_description<int>("motionMatchingSolver"), false},
  {ecs::get_type_description<Settings>("settings"), false},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
}, {},
{},
{},
animation_player_update_func, ecs::stage::act, ecs::tags::all, false);

void animation_player_update_func()
{
  ecs::perform_system(animation_player_update_descr, animation_player_update);
}

void update_bone_remap_func();

ecs::SystemDescription update_bone_remap_descr("update_bone_remap", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<bool>("bone_remaped"), false}
}, {
}, {"game","editor"},
{},
{},
update_bone_remap_func, ecs::stage::act, ecs::tags::all, false);

void update_bone_remap_func()
{
  ecs::perform_system(update_bone_remap_descr, update_bone_remap);
}

void init_animation_character_handler(const ecs::OnEntityCreated &event);
void init_animation_character_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> init_animation_character_descr("init_animation_character", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, {
}, {"game","editor"},
{},
{},
init_animation_character_handler, init_animation_character_singl_handler, ecs::tags::all);

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
}, {
}, {"game"},
{},
{},
init_animation_material_handler, init_animation_material_singl_handler, ecs::tags::all);

void init_animation_material_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, init_animation_material_descr, init_animation_material);
}
void init_animation_material_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_animation_material_descr, eid, init_animation_material);
}


