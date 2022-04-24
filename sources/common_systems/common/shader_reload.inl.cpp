#include "shader_reload.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription update_material_descr("update_material", {
  {ecs::get_type_description<Asset<Material>>("material"), false}
}, {
});

template<typename Callable>
void update_material(Callable lambda)
{
  ecs::perform_query<Asset<Material>&>
  (update_material_descr, lambda);
}


void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event);
void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::EntityId eid);

ecs::EventDescription<KeyDownEvent<SDLK_F5>> reload_shaders_descr("reload_shaders", {
}, {
}, {"editor","scene","game","scene","debug"},
{},
{},
reload_shaders_handler, reload_shaders_singl_handler, ecs::tags::all);

void reload_shaders_handler(const KeyDownEvent<SDLK_F5> &event)
{
  ecs::perform_event(event, reload_shaders_descr, reload_shaders);
}
void reload_shaders_singl_handler(const KeyDownEvent<SDLK_F5> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, reload_shaders_descr, eid, reload_shaders);
}

void load_directional_light_handler(const ecs::OnEntityCreated &event);
void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> load_directional_light_descr("load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, {
}, {"editor","scene","game"},
{},
{},
load_directional_light_handler, load_directional_light_singl_handler, ecs::tags::all);

void load_directional_light_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, load_directional_light_descr, load_directional_light);
}
void load_directional_light_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, load_directional_light_descr, eid, load_directional_light);
}

void reload_directional_light_handler(const ecs::OnEntityEdited &event);
void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityEdited> reload_directional_light_descr("reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, {
}, {"editor"},
{},
{},
reload_directional_light_handler, reload_directional_light_singl_handler, ecs::tags::all);

void reload_directional_light_handler(const ecs::OnEntityEdited &event)
{
  ecs::perform_event(event, reload_directional_light_descr, reload_directional_light);
}
void reload_directional_light_singl_handler(const ecs::OnEntityEdited &event, ecs::EntityId eid)
{
  ecs::perform_event(event, reload_directional_light_descr, eid, reload_directional_light);
}


