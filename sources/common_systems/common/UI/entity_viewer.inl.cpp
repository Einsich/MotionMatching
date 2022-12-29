#include "entity_viewer.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache entity_viewer__cache__;

static ecs::QueryCache init_imgui_style__cache__;

static void entity_viewer_implementation()
{
  ecs::perform_system(entity_viewer__cache__, entity_viewer);
}

static void init_imgui_style_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init_imgui_style__cache__, init_imgui_style);
}

static void init_imgui_style_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init_imgui_style__cache__, init_imgui_style);
}

static void registration_pull_entity_viewer()
{
  ecs::register_system(ecs::SystemDescription(
  "",
  "entity_viewer",
  &entity_viewer__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  "imgui_render",
  {},
  {},
  {"debug"},
  &entity_viewer_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "init_imgui_style",
  &init_imgui_style__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  {},
  {},
  {"debug"},
  &init_imgui_style_handler, &init_imgui_style_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_entity_viewer)
ECS_PULL_DEFINITION(variable_pull_entity_viewer)
