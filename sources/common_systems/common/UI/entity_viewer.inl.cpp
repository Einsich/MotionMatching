#include "entity_viewer.inl"
//Code-generator production

void entity_viewer_func();

ecs::SystemDescription entity_viewer_descr("entity_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, entity_viewer_func, ecs::SystemOrder::UI, ecs::SystemTag::GameEditor|ecs::SystemTag::Debug);

void entity_viewer_func()
{
  ecs::perform_system(entity_viewer_descr, entity_viewer);
}

void init_imgui_style_handler(const ecs::OnSceneCreated &event);
void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> init_imgui_style_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, init_imgui_style_handler, init_imgui_style_singl_handler, ecs::SystemTag::GameEditor|ecs::SystemTag::Debug);

void init_imgui_style_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_imgui_style_descr, init_imgui_style);
}
void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, init_imgui_style_descr, eid, init_imgui_style);
}


