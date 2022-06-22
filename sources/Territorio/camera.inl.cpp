#include "camera.inl"
#include <ecs_perform.h>
//Code-generator production

void camera_lerp_func();

ecs::SystemDescription camera_lerp_descr("camera_lerp", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<float>("lerpStrength"), false}
}, {
}, {},
{},
{},
camera_lerp_func, "act", {}, false);

void camera_lerp_func()
{
  ecs::perform_system(camera_lerp_descr, camera_lerp);
}

void setup_camera_handler(const ecs::Event &event);
void setup_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription setup_camera_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "setup_camera", {
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<float>("pixelPerUnit"), false},
  {ecs::get_type_description<float>("minZoom"), false},
  {ecs::get_type_description<float>("maxZoom"), false},
  {ecs::get_type_description<float>("curZoom"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, {
}, {},
{},
{},
setup_camera_handler, setup_camera_singl_handler, {});

void setup_camera_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, setup_camera_descr, setup_camera);
}
void setup_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, setup_camera_descr, eid, setup_camera);
}

void change_zoom_handler(const ecs::Event &event);
void change_zoom_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription change_zoom_descr(
  ecs::get_mutable_event_handlers<MouseWheelEvent>(), "change_zoom", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<float>("zoomStrength"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, {
}, {},
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

void move_camera_handler(const ecs::Event &event);
void move_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription move_camera_descr(
  ecs::get_mutable_event_handlers<MouseMoveEvent>(), "move_camera", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<bool>("cameraLocked"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, {
}, {},
{},
{},
move_camera_handler, move_camera_singl_handler, {});

void move_camera_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, move_camera_descr, move_camera);
}
void move_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, move_camera_descr, eid, move_camera);
}

void lock_unlock_camera_handler(const ecs::Event &event);
void lock_unlock_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription lock_unlock_camera_descr(
  ecs::get_mutable_event_handlers<MouseButtonDownEvent<MouseButton::LeftButton>>(), "lock_unlock_camera", {
  {ecs::get_type_description<bool>("cameraLocked"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, {
}, {},
{},
{},
lock_unlock_camera_handler, lock_unlock_camera_singl_handler, {});

void lock_unlock_camera_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseButtonDownEvent<MouseButton::LeftButton>&)event, lock_unlock_camera_descr, lock_unlock_camera);
}
void lock_unlock_camera_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseButtonDownEvent<MouseButton::LeftButton>&)event, lock_unlock_camera_descr, eid, lock_unlock_camera);
}


