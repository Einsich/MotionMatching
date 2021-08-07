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


void load_sky_box_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> load_sky_box_descr("load_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, load_sky_box_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void load_sky_box_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = load_sky_box_descr.begin(), end = load_sky_box_descr.end(); begin != end; ++begin)
  {
    load_sky_box(
      event,
      *begin.get_component<SkyBox>(0)
    );
  }
}


void reload_sky_box_handler(const ecs::OnEntityEdited &event);

ecs::EventDescription<ecs::OnEntityEdited> reload_sky_box_descr("reload_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, reload_sky_box_handler, (uint)(ecs::SystemTag::Editor));

void reload_sky_box_handler(const ecs::OnEntityEdited &event)
{
  for (ecs::QueryIterator begin = reload_sky_box_descr.begin(), end = reload_sky_box_descr.end(); begin != end; ++begin)
  {
    reload_sky_box(
      event,
      *begin.get_component<SkyBox>(0)
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


void load_sky_box_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> load_sky_box_singl_descr("load_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, load_sky_box_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void load_sky_box_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  load_sky_box(
    event,
      *begin.get_component<SkyBox>(0)
  );
}


void reload_sky_box_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityEdited> reload_sky_box_singl_descr("reload_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, reload_sky_box_singl_handler, (uint)(ecs::SystemTag::Editor));

void reload_sky_box_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin)
{
  reload_sky_box(
    event,
      *begin.get_component<SkyBox>(0)
  );
}



