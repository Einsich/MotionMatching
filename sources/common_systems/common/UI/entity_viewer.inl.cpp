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
void ecs_types_viewer_func();

ecs::SystemDescription ecs_types_viewer_descr("ecs_types_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, ecs_types_viewer_func, ecs::SystemOrder::UI, ecs::SystemTag::Editor);

void ecs_types_viewer_func()
{
  ecs::perform_system(ecs_types_viewer_descr, ecs_types_viewer);
}
void entity_creater_func();

ecs::SystemDescription entity_creater_descr("entity_creater", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, entity_creater_func, ecs::SystemOrder::UI, ecs::SystemTag::Editor);

void entity_creater_func()
{
  ecs::perform_system(entity_creater_descr, entity_creater);
}
void init_imgui_style_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_imgui_style_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, init_imgui_style_handler, ecs::SystemTag::GameEditor|ecs::SystemTag::Debug);

void init_imgui_style_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, init_imgui_style_descr, init_imgui_style);
}


void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_imgui_style_singl_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, init_imgui_style_singl_handler, ecs::SystemTag::GameEditor|ecs::SystemTag::Debug);

void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  init_imgui_style(
    event,
      *begin.get_component<EditorUI, 0>()
  );
}



