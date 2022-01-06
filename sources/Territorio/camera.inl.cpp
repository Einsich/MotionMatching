#include "camera.inl"
//Code-generator production

void camera_lerp_func();

ecs::SystemDescription camera_lerp_descr("camera_lerp", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<float>("lerpStrength"), false}
}, camera_lerp_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game);

void camera_lerp_func()
{
  ecs::perform_system(camera_lerp_descr, camera_lerp);
}

void setup_camera_handler(const ecs::OnEntityCreated &event);
void setup_camera_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> setup_camera_descr("setup_camera", {
  {ecs::get_type_description<mat3>("cameraProjection"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<float>("pixelPerUnit"), false},
  {ecs::get_type_description<float>("minZoom"), false},
  {ecs::get_type_description<float>("maxZoom"), false},
  {ecs::get_type_description<float>("curZoom"), false},
  {ecs::get_type_description<Transform2D>("transform"), false},
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false}
}, setup_camera_handler, setup_camera_singl_handler, ecs::SystemTag::Game);

void setup_camera_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, setup_camera_descr, setup_camera);
}
void setup_camera_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, setup_camera_descr, eid, setup_camera);
}

void change_zoom_handler(const MouseWheelEvent &event);
void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseWheelEvent> change_zoom_descr("change_zoom", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<float>("zoomStrength"), false},
  {ecs::get_type_description<vec3>("zoom"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, change_zoom_handler, change_zoom_singl_handler, ecs::SystemTag::Game);

void change_zoom_handler(const MouseWheelEvent &event)
{
  ecs::perform_event(event, change_zoom_descr, change_zoom);
}
void change_zoom_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, change_zoom_descr, eid, change_zoom);
}

void move_camera_handler(const MouseMoveEvent &event);
void move_camera_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseMoveEvent> move_camera_descr("move_camera", {
  {ecs::get_type_description<Transform2D>("targetTransform"), false},
  {ecs::get_type_description<WorldRenderer>("wr"), false},
  {ecs::get_type_description<bool>("cameraLocked"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, move_camera_handler, move_camera_singl_handler, ecs::SystemTag::Game);

void move_camera_handler(const MouseMoveEvent &event)
{
  ecs::perform_event(event, move_camera_descr, move_camera);
}
void move_camera_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, move_camera_descr, eid, move_camera);
}

void lock_unlock_camera_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);
void lock_unlock_camera_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> lock_unlock_camera_descr("lock_unlock_camera", {
  {ecs::get_type_description<bool>("cameraLocked"), false},
  {ecs::get_type_description<mat3>("cameraProjection"), false}
}, lock_unlock_camera_handler, lock_unlock_camera_singl_handler, ecs::SystemTag::Game);

void lock_unlock_camera_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  ecs::perform_event(event, lock_unlock_camera_descr, lock_unlock_camera);
}
void lock_unlock_camera_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, lock_unlock_camera_descr, eid, lock_unlock_camera);
}


