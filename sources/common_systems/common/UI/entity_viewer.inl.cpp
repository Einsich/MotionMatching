#include "entity_viewer.inl"
#include <ecs_perform.h>
//Code-generator production

void entity_viewer_func();

ecs::SystemDescription entity_viewer_descr("entity_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor","scene","debug"},
{},
{},
entity_viewer_func, "ui", {}, false);

void entity_viewer_func()
{
  ecs::perform_system(entity_viewer_descr, entity_viewer);
}

void init_imgui_style_handler(const ecs::Event &event);
void init_imgui_style_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription init_imgui_style_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, {
}, {"game","editor","scene","debug"},
{},
{},
init_imgui_style_handler, init_imgui_style_singl_handler, {});

void init_imgui_style_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_imgui_style_descr, init_imgui_style);
}
void init_imgui_style_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, init_imgui_style_descr, eid, init_imgui_style);
}


