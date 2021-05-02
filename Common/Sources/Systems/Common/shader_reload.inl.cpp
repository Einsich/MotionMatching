#include "shader_reload.inl"
//Code-generator production

void reload_shaders_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> reload_shaders_descr("reload_shaders", {
  {ecs::get_type_description<std::string>("project"), false}
}, reload_shaders_handler);

void reload_shaders_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = reload_shaders_descr.begin(), end = reload_shaders_descr.end(); begin != end; ++begin)
  {
    reload_shaders(
      event,
      *begin.get_component<std::string>(0)
    );
  }
}


void reload_shaders_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> reload_shaders_singl_descr("reload_shaders", {
  {ecs::get_type_description<std::string>("project"), false}
}, reload_shaders_singl_handler);

void reload_shaders_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  reload_shaders(
    event,
    *begin.get_component<std::string>(0)
  );
}



