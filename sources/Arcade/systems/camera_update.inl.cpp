#include "camera_update.inl"
//Code-generator production

ecs::QueryDescription update_main_camera_position_descr("update_main_camera_position", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
});

template<typename Callable>
void update_main_camera_position(Callable lambda)
{
  for (ecs::QueryIterator begin = update_main_camera_position_descr.begin(), end = update_main_camera_position_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<Transform2D>(0)
    );
  }
}


void update_camera_pos_before_render_func();

ecs::SystemDescription update_camera_pos_before_render_descr("update_camera_pos_before_render", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<ecs::Tag>("mainHero"), false}
}, update_camera_pos_before_render_func, ecs::SystemOrder::RENDER-1, ecs::SystemTag::Game);

void update_camera_pos_before_render_func()
{
  for (ecs::QueryIterator begin = update_camera_pos_before_render_descr.begin(), end = update_camera_pos_before_render_descr.end(); begin != end; ++begin)
  {
    update_camera_pos_before_render(
      *begin.get_component<Transform2D>(0)
    );
  }
}


void change_zoom_handler(const MouseWheelEvent &event);

ecs::EventDescription<MouseWheelEvent> change_zoom_descr("change_zoom", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, change_zoom_handler, ecs::SystemTag::Game);

void change_zoom_handler(const MouseWheelEvent &event)
{
  for (ecs::QueryIterator begin = change_zoom_descr.begin(), end = change_zoom_descr.end(); begin != end; ++begin)
  {
    change_zoom(
      event,
      *begin.get_component<Transform2D>(0),
      *begin.get_component<vec3>(1)
    );
  }
}


void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseWheelEvent> change_zoom_singl_descr("change_zoom", {
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, change_zoom_singl_handler, ecs::SystemTag::Game);

void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin)
{
  change_zoom(
    event,
      *begin.get_component<Transform2D>(0),
      *begin.get_component<vec3>(1)
  );
}



