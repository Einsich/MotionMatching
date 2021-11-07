#include "ecs_camera.inl"
//Code-generator production

ecs::QueryDescription find_all_created_camera_descr("find_all_created_camera", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<bool>("isEditorCamera"), true},
  {ecs::get_type_description<Camera>("camera"), false}
});

template<typename Callable>
void find_all_created_camera(Callable lambda)
{
  for (ecs::QueryIterator begin = find_all_created_camera_descr.begin(), end = find_all_created_camera_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<bool>(1),
       begin.get_component<bool>(2)
    );
  }
}


ecs::QueryDescription find_editor_camera_descr("find_editor_camera", {
  {ecs::get_type_description<bool>("isEditorCamera"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false}
});

template<typename Callable>
void find_editor_camera(Callable lambda)
{
  for (ecs::QueryIterator begin = find_editor_camera_descr.begin(), end = find_editor_camera_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<bool>(0),
      *begin.get_component<ecs::EntityId>(1)
    );
  }
}


ecs::SingleQueryDescription check_arcball_target_descr("check_arcball_target", {
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void check_arcball_target(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(check_arcball_target_descr, eid, begin))
  {
    lambda(
      *begin.get_component<Transform>(0)
    );
  }
}


ecs::SingleQueryDescription get_main_cam_query_descr("get_main_cam_query", {
  {ecs::get_type_description<Camera>("camera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void get_main_cam_query(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(get_main_cam_query_descr, eid, begin))
  {
    lambda(
      *begin.get_component<Camera>(0),
      *begin.get_component<Transform>(1)
    );
  }
}


void set_main_camera_func();

ecs::SystemDescription set_main_camera_descr("set_main_camera", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Camera>("camera"), false}
}, set_main_camera_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::GameEditor);

void set_main_camera_func()
{
  for (ecs::QueryIterator begin = set_main_camera_descr.begin(), end = set_main_camera_descr.end(); begin != end; ++begin)
  {
    set_main_camera(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<bool>(1),
      *begin.get_component<MainCamera>(2)
    );
  }
}


void arcball_camera_update_func();

ecs::SystemDescription arcball_camera_update_descr("arcball_camera_update", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::EntityId>("arcballCameraTarget"), false}
}, arcball_camera_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Game);

void arcball_camera_update_func()
{
  for (ecs::QueryIterator begin = arcball_camera_update_descr.begin(), end = arcball_camera_update_descr.end(); begin != end; ++begin)
  {
    arcball_camera_update(
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1),
      *begin.get_component<Transform>(2),
      *begin.get_component<ecs::EntityId>(3)
    );
  }
}


void freecamera_update_func();

ecs::SystemDescription freecamera_update_descr("freecamera_update", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, freecamera_update_func, ecs::SystemOrder::NO_ORDER, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecamera_update_func()
{
  for (ecs::QueryIterator begin = freecamera_update_descr.begin(), end = freecamera_update_descr.end(); begin != end; ++begin)
  {
    freecamera_update(
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1),
      *begin.get_component<Transform>(2)
    );
  }
}


void update_main_camera_game_transformations_func();

ecs::SystemDescription update_main_camera_game_transformations_descr("update_main_camera_game_transformations", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, update_main_camera_game_transformations_func, ecs::SystemOrder::RENDER - 5, ecs::SystemTag::Game);

void update_main_camera_game_transformations_func()
{
  for (ecs::QueryIterator begin = update_main_camera_game_transformations_descr.begin(), end = update_main_camera_game_transformations_descr.end(); begin != end; ++begin)
  {
    update_main_camera_game_transformations(
      *begin.get_component<MainCamera>(0)
    );
  }
}


void update_main_camera_editor_transformations_func();

ecs::SystemDescription update_main_camera_editor_transformations_descr("update_main_camera_editor_transformations", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<EditorCamera>("editorCamera"), false}
}, update_main_camera_editor_transformations_func, ecs::SystemOrder::RENDER - 5, ecs::SystemTag::Editor);

void update_main_camera_editor_transformations_func()
{
  for (ecs::QueryIterator begin = update_main_camera_editor_transformations_descr.begin(), end = update_main_camera_editor_transformations_descr.end(); begin != end; ++begin)
  {
    update_main_camera_editor_transformations(
      *begin.get_component<MainCamera>(0),
      *begin.get_component<EditorCamera>(1)
    );
  }
}


void find_main_camera_game_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> find_main_camera_game_descr("find_main_camera_game", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, find_main_camera_game_handler, ecs::SystemTag::Game);

void find_main_camera_game_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = find_main_camera_game_descr.begin(), end = find_main_camera_game_descr.end(); begin != end; ++begin)
  {
    find_main_camera_game(
      event,
      *begin.get_component<MainCamera>(0)
    );
  }
}


void find_main_camera_editor_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> find_main_camera_editor_descr("find_main_camera_editor", {
  {ecs::get_type_description<EditorCamera>("editorCameraManager"), false}
}, find_main_camera_editor_handler, ecs::SystemTag::Editor);

void find_main_camera_editor_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = find_main_camera_editor_descr.begin(), end = find_main_camera_editor_descr.end(); begin != end; ++begin)
  {
    find_main_camera_editor(
      event,
      *begin.get_component<EditorCamera>(0)
    );
  }
}


void arcball_created_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> arcball_created_descr("arcball_created", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_handler, ecs::SystemTag::Game);

void arcball_created_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = arcball_created_descr.begin(), end = arcball_created_descr.end(); begin != end; ++begin)
  {
    arcball_created(
      event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<Transform>(1)
    );
  }
}


void arccam_mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> arccam_mouse_move_handler_descr("arccam_mouse_move_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_move_handler_handler, ecs::SystemTag::Game);

void arccam_mouse_move_handler_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = arccam_mouse_move_handler_descr.begin(), end = arccam_mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    arccam_mouse_move_handler(
      event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
    );
  }
}


void arccam_mouse_click_handler_handler(const MouseClickEvent &event);

ecs::EventDescription<MouseClickEvent> arccam_mouse_click_handler_descr("arccam_mouse_click_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_click_handler_handler, ecs::SystemTag::Game);

void arccam_mouse_click_handler_handler(const MouseClickEvent &event)
{
  for (ecs::QueryIterator begin = arccam_mouse_click_handler_descr.begin(), end = arccam_mouse_click_handler_descr.end(); begin != end; ++begin)
  {
    arccam_mouse_click_handler(
      event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
    );
  }
}


void arccam_mouse_wheel_handler_handler(const MouseWheelEvent &event);

ecs::EventDescription<MouseWheelEvent> arccam_mouse_wheel_handler_descr("arccam_mouse_wheel_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_wheel_handler_handler, ecs::SystemTag::Game);

void arccam_mouse_wheel_handler_handler(const MouseWheelEvent &event)
{
  for (ecs::QueryIterator begin = arccam_mouse_wheel_handler_descr.begin(), end = arccam_mouse_wheel_handler_descr.end(); begin != end; ++begin)
  {
    arccam_mouse_wheel_handler(
      event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
    );
  }
}


void freecam_created_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> freecam_created_descr("freecam_created", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, freecam_created_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_created_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = freecam_created_descr.begin(), end = freecam_created_descr.end(); begin != end; ++begin)
  {
    freecam_created(
      event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<Transform>(1)
    );
  }
}


void freecam_mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> freecam_mouse_move_handler_descr("freecam_mouse_move_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_move_handler_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = freecam_mouse_move_handler_descr.begin(), end = freecam_mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    freecam_mouse_move_handler(
      event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<bool>(2)
    );
  }
}


void freecam_mouse_click_handler_handler(const MouseClickEvent &event);

ecs::EventDescription<MouseClickEvent> freecam_mouse_click_handler_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_click_handler_handler(const MouseClickEvent &event)
{
  for (ecs::QueryIterator begin = freecam_mouse_click_handler_descr.begin(), end = freecam_mouse_click_handler_descr.end(); begin != end; ++begin)
  {
    freecam_mouse_click_handler(
      event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1)
    );
  }
}


void find_main_camera_game_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> find_main_camera_game_singl_descr("find_main_camera_game", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, find_main_camera_game_singl_handler, ecs::SystemTag::Game);

void find_main_camera_game_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  find_main_camera_game(
    event,
      *begin.get_component<MainCamera>(0)
  );
}


void find_main_camera_editor_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> find_main_camera_editor_singl_descr("find_main_camera_editor", {
  {ecs::get_type_description<EditorCamera>("editorCameraManager"), false}
}, find_main_camera_editor_singl_handler, ecs::SystemTag::Editor);

void find_main_camera_editor_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  find_main_camera_editor(
    event,
      *begin.get_component<EditorCamera>(0)
  );
}


void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> arcball_created_singl_descr("arcball_created", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_singl_handler, ecs::SystemTag::Game);

void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  arcball_created(
    event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<Transform>(1)
  );
}


void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> arccam_mouse_move_handler_singl_descr("arccam_mouse_move_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_move_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  arccam_mouse_move_handler(
    event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
  );
}


void arccam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseClickEvent> arccam_mouse_click_handler_singl_descr("arccam_mouse_click_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_click_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin)
{
  arccam_mouse_click_handler(
    event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
  );
}


void arccam_mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseWheelEvent> arccam_mouse_wheel_handler_singl_descr("arccam_mouse_wheel_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_wheel_handler_singl_handler, ecs::SystemTag::Game);

void arccam_mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin)
{
  arccam_mouse_wheel_handler(
    event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
  );
}


void freecam_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> freecam_created_singl_descr("freecam_created", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, freecam_created_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  freecam_created(
    event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<Transform>(1)
  );
}


void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> freecam_mouse_move_handler_singl_descr("freecam_mouse_move_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_move_handler(
    event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<bool>(2)
  );
}


void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseClickEvent> freecam_mouse_click_handler_singl_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_singl_handler, ecs::SystemTag::Editor|ecs::SystemTag::Game);

void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_click_handler(
    event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1)
  );
}



