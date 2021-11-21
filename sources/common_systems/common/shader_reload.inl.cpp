#include "shader_reload.inl"
//Code-generator production

ecs::QueryDescription update_material_descr("update_material", {
  {ecs::get_type_description<Asset<Material>>("material"), false}
});

template<typename Callable>
void update_material(Callable lambda)
{
  ecs::perform_query<Asset<Material>&>
  (update_material_descr, lambda);
}


void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event);

ecs::EventDescription<KeyDownEvent<SDLK_F5>> reload_shaders_descr("reload_shaders", {
}, reload_shaders_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game| ecs::SystemTag::Debug);

void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event)
{
  ecs::perform_event(event, reload_shaders_descr, reload_shaders);
}


void load_directional_light_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> load_directional_light_descr("load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, load_directional_light_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void load_directional_light_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, load_directional_light_descr, load_directional_light);
}


void reload_directional_light_handler(const ecs::OnEntityEdited &event);

ecs::EventDescription<ecs::OnEntityEdited> reload_directional_light_descr("reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, reload_directional_light_handler, ecs::SystemTag::Editor);

void reload_directional_light_handler(const ecs::OnEntityEdited &event)
{
  ecs::perform_event(event, reload_directional_light_descr, reload_directional_light);
}


void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyDownEvent<SDLK_F5>> reload_shaders_singl_descr("reload_shaders", {
}, reload_shaders_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game| ecs::SystemTag::Debug);

void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::QueryIterator &)
{
  reload_shaders(
    event
  );
}


void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> load_directional_light_singl_descr("load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, load_directional_light_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  load_directional_light(
    event,
      *begin.get_component<DirectionLight, 0>()
  );
}


void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityEdited> reload_directional_light_singl_descr("reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, reload_directional_light_singl_handler, ecs::SystemTag::Editor);

void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::QueryIterator &begin)
{
  reload_directional_light(
    event,
      *begin.get_component<DirectionLight, 0>()
  );
}



