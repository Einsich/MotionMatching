#include "entity_viewer.inl"
//Code-generator production

void entity_viewer_func();

ecs::SystemDescription entity_viewer_descr("entity_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, entity_viewer_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::GameEditor));

void entity_viewer_func()
{
  for (ecs::QueryIterator begin = entity_viewer_descr.begin(), end = entity_viewer_descr.end(); begin != end; ++begin)
  {
    entity_viewer(
      *begin.get_component<EditorUI>(0)
    );
  }
}


void ecs_types_viewer_func();

ecs::SystemDescription ecs_types_viewer_descr("ecs_types_viewer", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, ecs_types_viewer_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::Editor));

void ecs_types_viewer_func()
{
  for (ecs::QueryIterator begin = ecs_types_viewer_descr.begin(), end = ecs_types_viewer_descr.end(); begin != end; ++begin)
  {
    ecs_types_viewer(
      *begin.get_component<EditorUI>(0)
    );
  }
}


void entity_creater_func();

ecs::SystemDescription entity_creater_descr("entity_creater", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, entity_creater_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::Editor));

void entity_creater_func()
{
  for (ecs::QueryIterator begin = entity_creater_descr.begin(), end = entity_creater_descr.end(); begin != end; ++begin)
  {
    entity_creater(
      *begin.get_component<EditorUI>(0)
    );
  }
}


void init_imgui_style_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> init_imgui_style_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, init_imgui_style_handler, (uint)(ecs::SystemTag::GameEditor));

void init_imgui_style_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = init_imgui_style_descr.begin(), end = init_imgui_style_descr.end(); begin != end; ++begin)
  {
    init_imgui_style(
      event,
      *begin.get_component<EditorUI>(0)
    );
  }
}


void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> init_imgui_style_singl_descr("init_imgui_style", {
  {ecs::get_type_description<EditorUI>("ui"), false}
}, init_imgui_style_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void init_imgui_style_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  init_imgui_style(
    event,
      *begin.get_component<EditorUI>(0)
  );
}



