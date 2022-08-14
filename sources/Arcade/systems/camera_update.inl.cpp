#include "camera_update.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription update_main_camera_position_descr("update_main_camera_position", {
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {-1u, ecs::get_name_hash("cameraProjection"), false}
}, {
});

template<typename Callable>
void update_main_camera_position(Callable lambda)
{
  ecs::perform_query<Transform2D&>
  (update_main_camera_position_descr, lambda);
}


void update_camera_pos_before_render_func();

ecs::SystemDescription update_camera_pos_before_render_descr("update_camera_pos_before_render", {
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {-1u, ecs::get_name_hash("mainHero"), false}
}, {
},
{},
{},
update_camera_pos_before_render_func, "before_render", {}, false);

void update_camera_pos_before_render_func()
{
  ecs::perform_system(update_camera_pos_before_render_descr, update_camera_pos_before_render);
}

void change_zoom_handler(const ecs::Event &event);
void change_zoom_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription change_zoom_descr(
  ecs::get_mutable_event_handlers<MouseWheelEvent>(), "change_zoom", {
  {ecs::get_type_hash<Transform2D>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<vec3>(), ecs::get_name_hash("zoom"), false},
  {-1u, ecs::get_name_hash("cameraProjection"), false}
}, {
},
{},
{},
change_zoom_handler, change_zoom_singl_handler, {});

void change_zoom_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseWheelEvent&)event, change_zoom_descr, change_zoom);
}
void change_zoom_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseWheelEvent&)event, change_zoom_descr, eid, change_zoom);
}


