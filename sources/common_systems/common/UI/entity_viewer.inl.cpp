#include "entity_viewer.inl"
#include <ecs_perform.h>
//Code-generator production

void entity_viewer_func();

ecs::SystemDescription entity_viewer_descr("entity_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor","scene","debug"},
entity_viewer_func, ecs::stage::ui, ecs::tags::all,
{},
{});

void entity_viewer_func()
{
  ecs::perform_system(entity_viewer_descr, entity_viewer);
}

void init_imgui_style_handler(const ecs::OnSceneCreated &event);
void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_imgui_style_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor","scene","debug"},
init_imgui_style_handler, init_imgui_style_singl_handler, ecs::tags::all);

void init_imgui_style_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_imgui_style_descr, init_imgui_style);
}
void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_imgui_style_descr, eid, init_imgui_style);
}


