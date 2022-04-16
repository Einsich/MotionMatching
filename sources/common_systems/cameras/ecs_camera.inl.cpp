#include "ecs_camera.inl"
//Code-generator production

ecs::QueryDescription find_all_created_camera_descr("find_all_created_camera", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Camera>("camera"), false}
}, {
});

template<typename Callable>
void find_all_created_camera(Callable lambda)
{
  ecs::perform_query<ecs::EntityId, bool>
  (find_all_created_camera_descr, lambda);
}


ecs::QueryDescription find_editor_camera_descr("find_editor_camera", {
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<Camera>("camera"), false}
}, {
});

template<typename Callable>
void find_editor_camera(Callable lambda)
{
  ecs::perform_query<bool, ecs::EntityId>
  (find_editor_camera_descr, lambda);
}


ecs::QueryDescription check_arcball_target_descr("check_arcball_target", {
  {ecs::get_type_description<Transform>("transform"), false}
}, {
});

template<typename Callable>
void check_arcball_target(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Transform&>
  (check_arcball_target_descr, eid, lambda);
}


ecs::QueryDescription get_main_cam_query_descr("get_main_cam_query", {
  {ecs::get_type_description<Camera>("camera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
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
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Camera>("camera"), false}
}, {
}, {},
set_main_camera_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor,
{},
{});

void set_main_camera_func()
{
  ecs::perform_system(set_main_camera_descr, set_main_camera);
}

void arcball_camera_update_func();

ecs::SystemDescription arcball_camera_update_descr("arcball_camera_update", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::EntityId>("arcballCameraTarget"), false}
}, {
}, {},
arcball_camera_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game,
{},
{});

void arcball_camera_update_func()
{
  ecs::perform_system(arcball_camera_update_descr, arcball_camera_update);
}

void freecamera_update_func();

ecs::SystemDescription freecamera_update_descr("freecamera_update", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, {
}, {},
freecamera_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Editor|ecs::SystemTag::Game,
{},
{});

void freecamera_update_func()
{
  ecs::perform_system(freecamera_update_descr, freecamera_update);
}

void update_main_camera_game_transformations_func();

ecs::SystemDescription update_main_camera_game_transformations_descr("update_main_camera_game_transformations", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
}, {},
update_main_camera_game_transformations_func, ecs::SystemOrder::RENDER - 5, ecs::SystemTag::Game,
{},
{});

void update_main_camera_game_transformations_func()
{
  ecs::perform_system(update_main_camera_game_transformations_descr, update_main_camera_game_transformations);
}

void update_main_camera_editor_transformations_func();

ecs::SystemDescription update_main_camera_editor_transformations_descr("update_main_camera_editor_transformations", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<EditorCamera>("editorCamera"), false}
}, {
}, {},
update_main_camera_editor_transformations_func, ecs::SystemOrder::RENDER - 5, ecs::SystemTag::Editor,
{},
{});

void update_main_camera_editor_transformations_func()
{
  ecs::perform_system(update_main_camera_editor_transformations_descr, update_main_camera_editor_transformations);
}

void find_main_camera_game_handler(const ecs::OnSceneCreated &event);
void find_main_camera_game_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> find_main_camera_game_descr("find_main_camera_game", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
}, {},
find_main_camera_game_handler, find_main_camera_game_singl_handler, ecs::SystemTag::Game);

void find_main_camera_game_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, find_main_camera_game_descr, find_main_camera_game);
}
void find_main_camera_game_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, find_main_camera_game_descr, eid, find_main_camera_game);
}

void find_main_camera_editor_handler(const ecs::OnSceneCreated &event);
void find_main_camera_editor_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> find_main_camera_editor_descr("find_main_camera_editor", {
  {ecs::get_type_description<EditorCamera>("editorCameraManager"), false}
}, {
}, {},
find_main_camera_editor_handler, find_main_camera_editor_singl_handler, ecs::SystemTag::Editor);

void find_main_camera_editor_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, find_main_camera_editor_descr, find_main_camera_editor);
}
void find_main_camera_editor_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, find_main_camera_editor_descr, eid, find_main_camera_editor);
}

void arcball_created_handler(const ecs::OnEntityCreated &event);
void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> arcball_created_descr("arcball_created", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, {
}, {},
arcball_created_handler, arcball_created_singl_handler, ecs::SystemTag::Game);

void arcball_created_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, arcball_created_descr, arcball_created);
}
void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, arcball_created_descr, eid, arcball_created);
}

void arccam_mouse_move_handler_handler(const MouseMoveEvent &event);
void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseMoveEvent> arccam_mouse_move_handler_descr("arccam_mouse_move_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, {
}, {},
arccam_mouse_move_handler_handler, arccam_mouse_move_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_move_handler_handler(const MouseMoveEvent &event)
{
  ecs::perform_event(event, arccam_mouse_move_handler_descr, arccam_mouse_move_handler);
}
void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, arccam_mouse_move_handler_descr, eid, arccam_mouse_move_handler);
}

void arccam_mouse_click_handler_handler(const MouseClickEvent &event);
void arccam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseClickEvent> arccam_mouse_click_handler_descr("arccam_mouse_click_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, {
}, {},
arccam_mouse_click_handler_handler, arccam_mouse_click_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_click_handler_handler(const MouseClickEvent &event)
{
  ecs::perform_event(event, arccam_mouse_click_handler_descr, arccam_mouse_click_handler);
}
void arccam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, arccam_mouse_click_handler_descr, eid, arccam_mouse_click_handler);
}

void arccam_mouse_wheel_handler_handler(const MouseWheelEvent &event);
void arccam_mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseWheelEvent> arccam_mouse_wheel_handler_descr("arccam_mouse_wheel_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, {
}, {},
arccam_mouse_wheel_handler_handler, arccam_mouse_wheel_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_wheel_handler_handler(const MouseWheelEvent &event)
{
  ecs::perform_event(event, arccam_mouse_wheel_handler_descr, arccam_mouse_wheel_handler);
}
void arccam_mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, arccam_mouse_wheel_handler_descr, eid, arccam_mouse_wheel_handler);
}

void freecam_created_handler(const ecs::OnEntityCreated &event);
void freecam_created_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> freecam_created_descr("freecam_created", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, {
}, {},
freecam_created_handler, freecam_created_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_created_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, freecam_created_descr, freecam_created);
}
void freecam_created_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, freecam_created_descr, eid, freecam_created);
}

void freecam_mouse_move_handler_handler(const MouseMoveEvent &event);
void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseMoveEvent> freecam_mouse_move_handler_descr("freecam_mouse_move_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, {
}, {},
freecam_mouse_move_handler_handler, freecam_mouse_move_handler_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_move_handler_handler(const MouseMoveEvent &event)
{
  ecs::perform_event(event, freecam_mouse_move_handler_descr, freecam_mouse_move_handler);
}
void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, freecam_mouse_move_handler_descr, eid, freecam_mouse_move_handler);
}

void freecam_mouse_click_handler_handler(const MouseClickEvent &event);
void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::EntityId eid);

ecs::EventDescription<MouseClickEvent> freecam_mouse_click_handler_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, {
}, {},
freecam_mouse_click_handler_handler, freecam_mouse_click_handler_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_click_handler_handler(const MouseClickEvent &event)
{
  ecs::perform_event(event, freecam_mouse_click_handler_descr, freecam_mouse_click_handler);
}
void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::EntityId eid)
{
  ecs::perform_event(event, freecam_mouse_click_handler_descr, eid, freecam_mouse_click_handler);
}


