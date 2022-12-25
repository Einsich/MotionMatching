#include "entity_viewer.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache init_imgui_style__cache__;

static ecs::QueryCache entity_viewer__cache__;

static void init_imgui_style_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), init_imgui_style__cache__, init_imgui_style);
}

static void init_imgui_style_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), init_imgui_style__cache__, init_imgui_style);
}

static void entity_viewer_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), entity_viewer__cache__, entity_viewer);
}

static void entity_viewer_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), entity_viewer__cache__, entity_viewer);
}

static void registration_pull_entity_viewer()
{
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

  ecs::register_event(ecs::EventDescription(
  "",
  "entity_viewer",
  &entity_viewer__cache__,
  {
    {"ui", ecs::get_type_index<EditorUI>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorUI>()}
  },
  {},
  {},
  {},
  {},
  {"debug"},
  &entity_viewer_handler, &entity_viewer_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_entity_viewer)
