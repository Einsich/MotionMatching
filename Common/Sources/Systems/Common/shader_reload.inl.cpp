#include "shader_reload.inl"
//Code-generator production

void reload_shaders_handler(const KeyEventAnyActionKey &event);

ecs::EventDescription<KeyEventAnyActionKey> reload_shaders_descr("reload_shaders", {
}, reload_shaders_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void reload_shaders_handler(const KeyEventAnyActionKey &event)
{
  for (ecs::QueryIterator begin = reload_shaders_descr.begin(), end = reload_shaders_descr.end(); begin != end; ++begin)
  {
    reload_shaders(
      event
    );
  }
}


void reload_shaders_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyEventAnyActionKey> reload_shaders_singl_descr("reload_shaders", {
}, reload_shaders_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void reload_shaders_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &)
{
  reload_shaders(
    event
  );
}



