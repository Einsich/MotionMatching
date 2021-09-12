#include "shader_reload.inl"
//Code-generator production

void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event);

ecs::EventDescription<KeyDownEvent<SDLK_F5>> reload_shaders_descr("reload_shaders", {
}, reload_shaders_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event)
{
  for (ecs::QueryIterator begin = reload_shaders_descr.begin(), end = reload_shaders_descr.end(); begin != end; ++begin)
  {
    reload_shaders(
      event
    );
  }
}


void load_directional_light_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> load_directional_light_descr("load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, load_directional_light_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void load_directional_light_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = load_directional_light_descr.begin(), end = load_directional_light_descr.end(); begin != end; ++begin)
  {
    load_directional_light(
      event,
      *begin.get_component<DirectionLight>(0)
    );
  }
}


void reload_directional_light_handler(const ecs::OnEntityEdited &event);

ecs::EventDescription<ecs::OnEntityEdited> reload_directional_light_descr("reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, reload_directional_light_handler, (uint)(ecs::SystemTag::Editor));

void reload_directional_light_handler(const ecs::OnEntityEdited &event)
{
  for (ecs::QueryIterator begin = reload_directional_light_descr.begin(), end = reload_directional_light_descr.end(); begin != end; ++begin)
  {
    reload_directional_light(
      event,
      *begin.get_component<DirectionLight>(0)
    );
  }
}


void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyDownEvent<SDLK_F5>> reload_shaders_singl_descr("reload_shaders", {
}, reload_shaders_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::QueryIterator &)
{
  reload_shaders(
    event
  );
}


void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> load_directional_light_singl_descr("load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, load_directional_light_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  load_directional_light(
    event,
      *begin.get_component<DirectionLight>(0)
  );
}


void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityEdited> reload_directional_light_singl_descr("reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, reload_directional_light_singl_handler, (uint)(ecs::SystemTag::Editor));

void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin)
{
  reload_directional_light(
    event,
      *begin.get_component<DirectionLight>(0)
  );
}



