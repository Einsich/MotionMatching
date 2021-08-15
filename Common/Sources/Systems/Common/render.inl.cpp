#include "render.inl"
//Code-generator production

void render_submenu_func();

ecs::SystemDescription render_submenu_descr("render_submenu", {
  {ecs::get_type_description<EditorRenderSettings>("settings"), false}
}, render_submenu_func, ecs::SystemOrder::UIMENU, (uint)( ecs::SystemTag::Editor));

void render_submenu_func()
{
  for (ecs::QueryIterator begin = render_submenu_descr.begin(), end = render_submenu_descr.end(); begin != end; ++begin)
  {
    render_submenu(
      *begin.get_component<EditorRenderSettings>(0)
    );
  }
}


void add_global_uniform_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> add_global_uniform_descr("add_global_uniform", {
}, add_global_uniform_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = add_global_uniform_descr.begin(), end = add_global_uniform_descr.end(); begin != end; ++begin)
  {
    add_global_uniform(
      event
    );
  }
}


void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> add_global_uniform_singl_descr("add_global_uniform", {
}, add_global_uniform_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  add_global_uniform(
    event
  );
}



