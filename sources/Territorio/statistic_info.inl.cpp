#include "statistic_info.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_lands_descr("gather_lands", {
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<int>("landCount"), false},
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayableLand"), false}
}, {
});

template<typename Callable>
void gather_lands(Callable lambda)
{
  ecs::perform_query<uint, int, uint>
  (gather_lands_descr, lambda);
}


void show_statistic_func();

ecs::SystemDescription show_statistic_descr("show_statistic", {
  {ecs::get_type_description<ecs::vector<vec3>>("land_colors"), false}
}, {
},
{},
{},
show_statistic_func, "ui", {}, false);

void show_statistic_func()
{
  ecs::perform_system(show_statistic_descr, show_statistic);
}

void move_text_handler(const ecs::Event &event);
void move_text_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription move_text_descr(
  ecs::get_mutable_event_handlers<MouseMoveEvent>(), "move_text", {
  {ecs::get_type_description<ivec2>("position"), false},
  {ecs::get_type_description<ecs::string>("text"), false}
}, {
},
{},
{},
move_text_handler, move_text_singl_handler, {});

void move_text_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, move_text_descr, move_text);
}
void move_text_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, move_text_descr, eid, move_text);
}


