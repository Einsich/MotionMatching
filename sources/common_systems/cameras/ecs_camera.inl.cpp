#include "ecs_camera.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription find_all_created_camera_descr("find_all_created_camera", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false},
  {-1u, ecs::get_name_hash("camera"), false}
}, {
});

template<typename Callable>
void find_all_created_camera(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, bool>
  (find_all_created_camera_descr, lambda);
}


ecs::QueryDescription find_editor_camera_descr("find_editor_camera", {
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false},
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {-1u, ecs::get_name_hash("camera"), false}
}, {
});

template<typename Callable>
void find_editor_camera(Callable lambda)
{
  ecs::perform_query<bool, ecs::EntityId>
  (find_editor_camera_descr, lambda);
}


ecs::QueryDescription check_arcball_target_descr("check_arcball_target", {
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false}
}, {
});

template<typename Callable>
void check_arcball_target(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Transform&>
  (check_arcball_target_descr, eid, lambda);
}


ecs::QueryDescription get_main_cam_query_descr("get_main_cam_query", {
  {ecs::get_type_hash<Camera>(), ecs::get_name_hash("camera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false}
}, {
});

template<typename Callable>
void get_main_cam_query(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Camera&, Transform&>
  (get_main_cam_query_descr, eid, lambda);
}


void set_main_camera_func();

ecs::SystemDescription set_main_camera_descr("set_main_camera", {
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("eid"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false},
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false},
  {-1u, ecs::get_name_hash("camera"), false}
}, {
},
{},
{},
set_main_camera_func, "act", {}, false);

void set_main_camera_func()
{
  ecs::perform_system(set_main_camera_descr, set_main_camera);
}

void arcball_camera_update_func();

ecs::SystemDescription arcball_camera_update_descr("arcball_camera_update", {
  {ecs::get_type_hash<ArcballCamera>(), ecs::get_name_hash("arcballCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<ecs::EntityId>(), ecs::get_name_hash("arcballCameraTarget"), false}
}, {
},
{},
{},
arcball_camera_update_func, "act", {}, false);

void arcball_camera_update_func()
{
  ecs::perform_system(arcball_camera_update_descr, arcball_camera_update);
}

void freecamera_update_func();

ecs::SystemDescription freecamera_update_descr("freecamera_update", {
  {ecs::get_type_hash<FreeCamera>(), ecs::get_name_hash("freeCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false}
}, {
},
{},
{},
freecamera_update_func, "act", {}, false);

void freecamera_update_func()
{
  ecs::perform_system(freecamera_update_descr, freecamera_update);
}

void update_main_camera_game_transformations_func();

ecs::SystemDescription update_main_camera_game_transformations_descr("update_main_camera_game_transformations", {
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false}
}, {
},
{},
{},
update_main_camera_game_transformations_func, "before_render", {"game"}, false);

void update_main_camera_game_transformations_func()
{
  ecs::perform_system(update_main_camera_game_transformations_descr, update_main_camera_game_transformations);
}

void update_main_camera_editor_transformations_func();

ecs::SystemDescription update_main_camera_editor_transformations_descr("update_main_camera_editor_transformations", {
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false},
  {ecs::get_type_hash<EditorCamera>(), ecs::get_name_hash("editorCamera"), false}
}, {
},
{},
{},
update_main_camera_editor_transformations_func, "before_render", {"editor"}, false);

void update_main_camera_editor_transformations_func()
{
  ecs::perform_system(update_main_camera_editor_transformations_descr, update_main_camera_editor_transformations);
}

void find_main_camera_game_handler(const ecs::Event &event);
void find_main_camera_game_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription find_main_camera_game_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "find_main_camera_game", {
  {ecs::get_type_hash<MainCamera>(), ecs::get_name_hash("mainCamera"), false}
}, {
},
{},
{},
find_main_camera_game_handler, find_main_camera_game_singl_handler, {"game"});

void find_main_camera_game_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, find_main_camera_game_descr, find_main_camera_game);
}
void find_main_camera_game_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, find_main_camera_game_descr, eid, find_main_camera_game);
}

void find_main_camera_editor_handler(const ecs::Event &event);
void find_main_camera_editor_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription find_main_camera_editor_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "find_main_camera_editor", {
  {ecs::get_type_hash<EditorCamera>(), ecs::get_name_hash("editorCameraManager"), false}
}, {
},
{},
{},
find_main_camera_editor_handler, find_main_camera_editor_singl_handler, {"editor"});

void find_main_camera_editor_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, find_main_camera_editor_descr, find_main_camera_editor);
}
void find_main_camera_editor_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, find_main_camera_editor_descr, eid, find_main_camera_editor);
}

void arcball_created_handler(const ecs::Event &event);
void arcball_created_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription arcball_created_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "arcball_created", {
  {ecs::get_type_hash<ArcballCamera>(), ecs::get_name_hash("arcballCamera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false}
}, {
},
{},
{},
arcball_created_handler, arcball_created_singl_handler, {});

void arcball_created_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, arcball_created_descr, arcball_created);
}
void arcball_created_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, arcball_created_descr, eid, arcball_created);
}

void arccam_mouse_move_handler_handler(const ecs::Event &event);
void arccam_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription arccam_mouse_move_handler_descr(
  ecs::get_mutable_event_handlers<MouseMoveEvent>(), "arccam_mouse_move_handler", {
  {ecs::get_type_hash<ArcballCamera>(), ecs::get_name_hash("arcballCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false}
}, {
},
{},
{},
arccam_mouse_move_handler_handler, arccam_mouse_move_handler_singl_handler, {});

void arccam_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, arccam_mouse_move_handler_descr, arccam_mouse_move_handler);
}
void arccam_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, arccam_mouse_move_handler_descr, eid, arccam_mouse_move_handler);
}

void arccam_mouse_click_handler_handler(const ecs::Event &event);
void arccam_mouse_click_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription arccam_mouse_click_handler_descr(
  ecs::get_mutable_event_handlers<MouseClickEvent>(), "arccam_mouse_click_handler", {
  {ecs::get_type_hash<ArcballCamera>(), ecs::get_name_hash("arcballCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false}
}, {
},
{},
{},
arccam_mouse_click_handler_handler, arccam_mouse_click_handler_singl_handler, {});

void arccam_mouse_click_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseClickEvent&)event, arccam_mouse_click_handler_descr, arccam_mouse_click_handler);
}
void arccam_mouse_click_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseClickEvent&)event, arccam_mouse_click_handler_descr, eid, arccam_mouse_click_handler);
}

void arccam_mouse_wheel_handler_handler(const ecs::Event &event);
void arccam_mouse_wheel_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription arccam_mouse_wheel_handler_descr(
  ecs::get_mutable_event_handlers<MouseWheelEvent>(), "arccam_mouse_wheel_handler", {
  {ecs::get_type_hash<ArcballCamera>(), ecs::get_name_hash("arcballCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false}
}, {
},
{},
{},
arccam_mouse_wheel_handler_handler, arccam_mouse_wheel_handler_singl_handler, {});

void arccam_mouse_wheel_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseWheelEvent&)event, arccam_mouse_wheel_handler_descr, arccam_mouse_wheel_handler);
}
void arccam_mouse_wheel_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseWheelEvent&)event, arccam_mouse_wheel_handler_descr, eid, arccam_mouse_wheel_handler);
}

void freecam_created_handler(const ecs::Event &event);
void freecam_created_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription freecam_created_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "freecam_created", {
  {ecs::get_type_hash<FreeCamera>(), ecs::get_name_hash("freeCamera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false}
}, {
},
{},
{},
freecam_created_handler, freecam_created_singl_handler, {});

void freecam_created_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, freecam_created_descr, freecam_created);
}
void freecam_created_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, freecam_created_descr, eid, freecam_created);
}

void freecam_mouse_move_handler_handler(const ecs::Event &event);
void freecam_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription freecam_mouse_move_handler_descr(
  ecs::get_mutable_event_handlers<MouseMoveEvent>(), "freecam_mouse_move_handler", {
  {ecs::get_type_hash<FreeCamera>(), ecs::get_name_hash("freeCamera"), false},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false}
}, {
},
{},
{},
freecam_mouse_move_handler_handler, freecam_mouse_move_handler_singl_handler, {});

void freecam_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, freecam_mouse_move_handler_descr, freecam_mouse_move_handler);
}
void freecam_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, freecam_mouse_move_handler_descr, eid, freecam_mouse_move_handler);
}

void freecam_mouse_click_handler_handler(const ecs::Event &event);
void freecam_mouse_click_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription freecam_mouse_click_handler_descr(
  ecs::get_mutable_event_handlers<MouseClickEvent>(), "freecam_mouse_click_handler", {
  {ecs::get_type_hash<FreeCamera>(), ecs::get_name_hash("freeCamera"), false},
  {ecs::get_type_hash<bool>(), ecs::get_name_hash("isMainCamera"), false}
}, {
},
{},
{},
freecam_mouse_click_handler_handler, freecam_mouse_click_handler_singl_handler, {});

void freecam_mouse_click_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseClickEvent&)event, freecam_mouse_click_handler_descr, freecam_mouse_click_handler);
}
void freecam_mouse_click_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseClickEvent&)event, freecam_mouse_click_handler_descr, eid, freecam_mouse_click_handler);
}


