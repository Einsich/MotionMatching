#include "camera_update.inl"
//Code-generator production

ecs::QueryDescription update_main_camera_position_descr("update_main_camera_position", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, {
});

template<typename Callable>
void update_main_camera_position(Callable lambda)
{
  ecs::perform_query<Transform2D&>
  (update_main_camera_position_descr, lambda);
}


void change_zoom_handler(const MouseWheelEvent &event);
void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseWheelEvent> change_zoom_descr("change_zoom", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, {
}, {},
change_zoom_handler, change_zoom_singl_handler, ecs::tags::all);

void change_zoom_handler(const MouseWheelEvent &event)
{
  ecs::perform_event(event, change_zoom_descr, change_zoom);
}
void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, change_zoom_descr, eid, change_zoom);
}


