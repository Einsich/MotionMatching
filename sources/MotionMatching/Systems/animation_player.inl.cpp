#include "animation_player.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache animation_player_update__cache__;

static ecs::QueryCache update_bone_remap__cache__;

static ecs::QueryCache init_animation_character__cache__;

static ecs::QueryCache init_animation_material__cache__;

static void animation_player_update_implementation()
{
  ecs::perform_system(animation_player_update__cache__, animation_player_update);
}

static void update_bone_remap_implementation()
{
  ecs::perform_system(update_bone_remap__cache__, update_bone_remap);
}

static void init_animation_character_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), init_animation_character__cache__, init_animation_character);
}

static void init_animation_character_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), init_animation_character__cache__, init_animation_character);
}

static void init_animation_material_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), init_animation_material__cache__, init_animation_material);
}

static void init_animation_material_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), init_animation_material__cache__, init_animation_material);
}

static void registration_pull_animation_player()
{
  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/animation_player.inl:10",
  "animation_player_update",
  &animation_player_update__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &animation_player_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/animation_player.inl:50",
  "update_bone_remap",
  &update_bone_remap__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()},
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Mesh>>()},
    {"bone_remaped", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &update_bone_remap_implementation));

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/animation_player.inl:28",
  "init_animation_character",
  &init_animation_character__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()}
  },
  {},
  {},
  {},
  {},
  {},
  &init_animation_character_handler, &init_animation_character_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/animation_player.inl:68",
  "init_animation_material",
  &init_animation_material__cache__,
  {
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()}
  },
  {},
  {},
  {},
  {},
  {"game"},
  &init_animation_material_handler, &init_animation_material_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_animation_player)
ECS_PULL_DEFINITION(variable_pull_animation_player)
