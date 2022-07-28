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


void reload_shaders_handler(const ecs::Event &event);
void reload_shaders_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription reload_shaders_descr(
  ecs::get_mutable_event_handlers<KeyDownEvent<SDLK_F5>>(), "reload_shaders", {
}, {
},
{},
{},
reload_shaders_handler, reload_shaders_singl_handler, {"debug"});

void reload_shaders_handler(const ecs::Event &event)
{
  ecs::perform_event((const KeyDownEvent<SDLK_F5>&)event, reload_shaders_descr, reload_shaders);
}
void reload_shaders_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const KeyDownEvent<SDLK_F5>&)event, reload_shaders_descr, eid, reload_shaders);
}

void load_directional_light_handler(const ecs::Event &event);
void load_directional_light_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription load_directional_light_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "load_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, {
},
{},
{},
load_directional_light_handler, load_directional_light_singl_handler, {});

void load_directional_light_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, load_directional_light_descr, load_directional_light);
}
void load_directional_light_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, load_directional_light_descr, eid, load_directional_light);
}

void reload_directional_light_handler(const ecs::Event &event);
void reload_directional_light_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription reload_directional_light_descr(
  ecs::get_mutable_event_handlers<OnEntityEdited>(), "reload_directional_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, {
},
{},
{},
reload_directional_light_handler, reload_directional_light_singl_handler, {"editor"});

void reload_directional_light_handler(const ecs::Event &event)
{
  ecs::perform_event((const OnEntityEdited&)event, reload_directional_light_descr, reload_directional_light);
}
void reload_directional_light_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const OnEntityEdited&)event, reload_directional_light_descr, eid, reload_directional_light);
}


