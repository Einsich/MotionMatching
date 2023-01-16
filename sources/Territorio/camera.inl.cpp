#include "camera.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache camera_lerp__cache__;

static ecs::QueryCache setup_camera__cache__;

static ecs::QueryCache change_zoom__cache__;

static ecs::QueryCache move_camera__cache__;

static ecs::QueryCache lock_unlock_camera__cache__;

static void camera_lerp_implementation()
{
  ecs::perform_system(camera_lerp__cache__, camera_lerp);
}

static void setup_camera_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), setup_camera__cache__, setup_camera);
}

static void setup_camera_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), setup_camera__cache__, setup_camera);
}

static void change_zoom_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseWheelEvent &>(event), change_zoom__cache__, change_zoom);
}

static void change_zoom_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseWheelEvent &>(event), change_zoom__cache__, change_zoom);
}

static void move_camera_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), move_camera__cache__, move_camera);
}

static void move_camera_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), move_camera__cache__, move_camera);
}

static void lock_unlock_camera_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), lock_unlock_camera__cache__, lock_unlock_camera);
}

static void lock_unlock_camera_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseButtonDownEvent<MouseButton::LeftButton> &>(event), lock_unlock_camera__cache__, lock_unlock_camera);
}

static void registration_pull_camera()
{
  ecs::register_system(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/camera.inl:33",
  "camera_lerp",
  &camera_lerp__cache__,
  {
    {"targetTransform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"lerpStrength", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &camera_lerp_implementation);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/camera.inl:8",
  "setup_camera",
  &setup_camera__cache__,
  {
    {"cameraProjection", ecs::get_type_index<mat3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<mat3>()},
    {"zoom", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()},
    {"pixelPerUnit", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"minZoom", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"maxZoom", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"curZoom", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"targetTransform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<WorldRenderer>()}
  },
  {},
  {},
  {},
  {},
  {},
  &setup_camera_handler, &setup_camera_single_handler,
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/camera.inl:44",
  "change_zoom",
  &change_zoom__cache__,
  {
    {"targetTransform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"zoomStrength", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"zoom", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()}
  },
  {
    {"cameraProjection", ecs::TypeIndex<mat3>::value}
  },
  {},
  {},
  {},
  {},
  &change_zoom_handler, &change_zoom_single_handler,
  ecs::EventIndex<MouseWheelEvent>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/camera.inl:56",
  "move_camera",
  &move_camera__cache__,
  {
    {"targetTransform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"wr", ecs::get_type_index<WorldRenderer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<WorldRenderer>()},
    {"cameraLocked", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {
    {"cameraProjection", ecs::TypeIndex<mat3>::value}
  },
  {},
  {},
  {},
  {},
  &move_camera_handler, &move_camera_single_handler,
  ecs::EventIndex<MouseMoveEvent>::value);

  ecs::register_event(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/Territorio/camera.inl:71",
  "lock_unlock_camera",
  &lock_unlock_camera__cache__,
  {
    {"cameraLocked", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"cameraProjection", ecs::TypeIndex<mat3>::value}
  },
  {},
  {},
  {},
  {},
  &lock_unlock_camera_handler, &lock_unlock_camera_single_handler,
  ecs::EventIndex<MouseButtonDownEvent<MouseButton::LeftButton>>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_camera)
ECS_PULL_DEFINITION(variable_pull_camera)
