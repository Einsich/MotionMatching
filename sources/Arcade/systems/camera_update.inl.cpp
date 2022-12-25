#include "camera_update.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache update_main_camera_position__cache__;

static ecs::QueryCache update_camera_pos_before_render__cache__;

static ecs::QueryCache change_zoom__cache__;

template<typename Callable>
static void update_main_camera_position(Callable lambda)
{
  ecs::perform_query<Transform2D&>(update_main_camera_position__cache__, lambda);
}

static void update_camera_pos_before_render_implementation()
{
  ecs::perform_system(update_camera_pos_before_render__cache__, update_camera_pos_before_render);
}

static void change_zoom_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseWheelEvent &>(event), change_zoom__cache__, change_zoom);
}

static void change_zoom_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseWheelEvent &>(event), change_zoom__cache__, change_zoom);
}

static void registration_pull_camera_update()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "update_main_camera_position",
  &update_main_camera_position__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()}
  },
  {
    {"cameraProjection", ecs::TypeIndex<mat3>::value}
  },
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "update_camera_pos_before_render",
  &update_camera_pos_before_render__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform2D>()}
  },
  {
    {"mainHero", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  {"before_render_end_sync_point"},
  {"before_render_begin_sync_point"},
  {},
  &update_camera_pos_before_render_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "change_zoom",
  &change_zoom__cache__,
  {
    {"transform", ecs::get_type_index<Transform2D>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform2D>()},
    {"zoom", ecs::get_type_index<vec3>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<vec3>()}
  },
  {
    {"cameraProjection", ecs::TypeIndex<mat3>::value}
  },
  {},
  {},
  {},
  {},
  &change_zoom_handler, &change_zoom_single_handler),
  ecs::EventIndex<MouseWheelEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_camera_update)
