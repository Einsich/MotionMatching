#include "ecs_camera.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache find_all_created_camera__cache__;

static ecs::QueryCache find_editor_camera__cache__;

static ecs::QueryCache check_arcball_target__cache__;

static ecs::QueryCache get_main_cam_query__cache__;

static ecs::QueryCache set_main_camera__cache__;

static ecs::QueryCache arcball_camera_update__cache__;

static ecs::QueryCache freecamera_update__cache__;

static ecs::QueryCache update_main_camera_game_transformations__cache__;

static ecs::QueryCache update_main_camera_editor_transformations__cache__;

static ecs::QueryCache find_main_camera_game__cache__;

static ecs::QueryCache find_main_camera_editor__cache__;

static ecs::QueryCache arcball_created__cache__;

static ecs::QueryCache arccam_mouse_move_handler__cache__;

static ecs::QueryCache arccam_mouse_click_handler__cache__;

static ecs::QueryCache arccam_mouse_wheel_handler__cache__;

static ecs::QueryCache freecam_created__cache__;

static ecs::QueryCache freecam_mouse_move_handler__cache__;

static ecs::QueryCache freecam_mouse_click_handler__cache__;

template<typename Callable>
static void find_all_created_camera(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, bool>(find_all_created_camera__cache__, lambda);
}

template<typename Callable>
static void find_editor_camera(Callable lambda)
{
  ecs::perform_query<bool, ecs::EntityId>(find_editor_camera__cache__, lambda);
}

template<typename Callable>
static void check_arcball_target(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Transform&>(check_arcball_target__cache__, eid, lambda);
}

template<typename Callable>
static void get_main_cam_query(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Camera&, Transform&>(get_main_cam_query__cache__, eid, lambda);
}

static void set_main_camera_implementation()
{
  ecs::perform_system(set_main_camera__cache__, set_main_camera);
}

static void arcball_camera_update_implementation()
{
  ecs::perform_system(arcball_camera_update__cache__, arcball_camera_update);
}

static void freecamera_update_implementation()
{
  ecs::perform_system(freecamera_update__cache__, freecamera_update);
}

static void update_main_camera_game_transformations_implementation()
{
  ecs::perform_system(update_main_camera_game_transformations__cache__, update_main_camera_game_transformations);
}

static void update_main_camera_editor_transformations_implementation()
{
  ecs::perform_system(update_main_camera_editor_transformations__cache__, update_main_camera_editor_transformations);
}

static void find_main_camera_game_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), find_main_camera_game__cache__, find_main_camera_game);
}

static void find_main_camera_game_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), find_main_camera_game__cache__, find_main_camera_game);
}

static void find_main_camera_editor_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), find_main_camera_editor__cache__, find_main_camera_editor);
}

static void find_main_camera_editor_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), find_main_camera_editor__cache__, find_main_camera_editor);
}

static void arcball_created_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), arcball_created__cache__, arcball_created);
}

static void arcball_created_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), arcball_created__cache__, arcball_created);
}

static void arccam_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), arccam_mouse_move_handler__cache__, arccam_mouse_move_handler);
}

static void arccam_mouse_move_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), arccam_mouse_move_handler__cache__, arccam_mouse_move_handler);
}

static void arccam_mouse_click_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseClickEvent &>(event), arccam_mouse_click_handler__cache__, arccam_mouse_click_handler);
}

static void arccam_mouse_click_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseClickEvent &>(event), arccam_mouse_click_handler__cache__, arccam_mouse_click_handler);
}

static void arccam_mouse_wheel_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseWheelEvent &>(event), arccam_mouse_wheel_handler__cache__, arccam_mouse_wheel_handler);
}

static void arccam_mouse_wheel_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseWheelEvent &>(event), arccam_mouse_wheel_handler__cache__, arccam_mouse_wheel_handler);
}

static void freecam_created_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), freecam_created__cache__, freecam_created);
}

static void freecam_created_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), freecam_created__cache__, freecam_created);
}

static void freecam_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), freecam_mouse_move_handler__cache__, freecam_mouse_move_handler);
}

static void freecam_mouse_move_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), freecam_mouse_move_handler__cache__, freecam_mouse_move_handler);
}

static void freecam_mouse_click_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseClickEvent &>(event), freecam_mouse_click_handler__cache__, freecam_mouse_click_handler);
}

static void freecam_mouse_click_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseClickEvent &>(event), freecam_mouse_click_handler__cache__, freecam_mouse_click_handler);
}

static void registration_pull_ecs_camera()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "find_all_created_camera",
  &find_all_created_camera__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {
    {"camera", ecs::TypeIndex<Camera>::value}
  },
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "find_editor_camera",
  &find_editor_camera__cache__,
  {
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()}
  },
  {
    {"camera", ecs::TypeIndex<Camera>::value}
  },
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "check_arcball_target",
  &check_arcball_target__cache__,
  {
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "",
  "get_main_cam_query",
  &get_main_cam_query__cache__,
  {
    {"camera", ecs::get_type_index<Camera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Camera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "set_main_camera",
  &set_main_camera__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MainCamera>()}
  },
  {
    {"camera", ecs::TypeIndex<Camera>::value}
  },
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &set_main_camera_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "arcball_camera_update",
  &arcball_camera_update__cache__,
  {
    {"arcballCamera", ecs::get_type_index<ArcballCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ArcballCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"arcballCameraTarget", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &arcball_camera_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "freecamera_update",
  &freecamera_update__cache__,
  {
    {"freeCamera", ecs::get_type_index<FreeCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<FreeCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {},
  {"act_end_sync_point"},
  {"act_begin_sync_point"},
  {},
  &freecamera_update_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "update_main_camera_game_transformations",
  &update_main_camera_game_transformations__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MainCamera>()}
  },
  {},
  {},
  {"before_render_end_sync_point"},
  {"before_render_begin_sync_point"},
  {"game"},
  &update_main_camera_game_transformations_implementation));

  ecs::register_system(ecs::SystemDescription(
  "",
  "update_main_camera_editor_transformations",
  &update_main_camera_editor_transformations__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MainCamera>()},
    {"editorCamera", ecs::get_type_index<EditorCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorCamera>()}
  },
  {},
  {},
  {"before_render_end_sync_point"},
  {"before_render_begin_sync_point"},
  {"editor"},
  &update_main_camera_editor_transformations_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "find_main_camera_game",
  &find_main_camera_game__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<MainCamera>()}
  },
  {},
  {},
  {},
  {},
  {"game"},
  &find_main_camera_game_handler, &find_main_camera_game_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "find_main_camera_editor",
  &find_main_camera_editor__cache__,
  {
    {"editorCameraManager", ecs::get_type_index<EditorCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorCamera>()}
  },
  {},
  {},
  {},
  {},
  {"editor"},
  &find_main_camera_editor_handler, &find_main_camera_editor_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "arcball_created",
  &arcball_created__cache__,
  {
    {"arcballCamera", ecs::get_type_index<ArcballCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ArcballCamera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {},
  {},
  {},
  {},
  &arcball_created_handler, &arcball_created_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "arccam_mouse_move_handler",
  &arccam_mouse_move_handler__cache__,
  {
    {"arcballCamera", ecs::get_type_index<ArcballCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ArcballCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {},
  {},
  {},
  &arccam_mouse_move_handler_handler, &arccam_mouse_move_handler_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "arccam_mouse_click_handler",
  &arccam_mouse_click_handler__cache__,
  {
    {"arcballCamera", ecs::get_type_index<ArcballCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ArcballCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {},
  {},
  {},
  &arccam_mouse_click_handler_handler, &arccam_mouse_click_handler_single_handler),
  ecs::EventIndex<MouseClickEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "arccam_mouse_wheel_handler",
  &arccam_mouse_wheel_handler__cache__,
  {
    {"arcballCamera", ecs::get_type_index<ArcballCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ArcballCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {},
  {},
  {},
  &arccam_mouse_wheel_handler_handler, &arccam_mouse_wheel_handler_single_handler),
  ecs::EventIndex<MouseWheelEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "freecam_created",
  &freecam_created__cache__,
  {
    {"freeCamera", ecs::get_type_index<FreeCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<FreeCamera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {},
  {},
  {},
  {},
  &freecam_created_handler, &freecam_created_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "freecam_mouse_move_handler",
  &freecam_mouse_move_handler__cache__,
  {
    {"freeCamera", ecs::get_type_index<FreeCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<FreeCamera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {},
  {},
  {},
  &freecam_mouse_move_handler_handler, &freecam_mouse_move_handler_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "freecam_mouse_click_handler",
  &freecam_mouse_click_handler__cache__,
  {
    {"freeCamera", ecs::get_type_index<FreeCamera>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<FreeCamera>()},
    {"isMainCamera", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  {},
  {},
  {},
  &freecam_mouse_click_handler_handler, &freecam_mouse_click_handler_single_handler),
  ecs::EventIndex<MouseClickEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_ecs_camera)
