#include "ecs_camera.inl"
//Code-generator production

ecs::QueryDescription find_all_created_camera_descr("find_all_created_camera", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Camera>("camera"), false}
});

template<typename Callable>
void find_all_created_camera(Callable lambda)
{
  for (ecs::QueryIterator begin = find_all_created_camera_descr.begin(), end = find_all_created_camera_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<bool>(1)
    );
  }
}


ecs::QueryDescription register_cam_query_descr("register_cam_query", {
  {ecs::get_type_description<CameraManager>("manager"), false}
});

template<typename Callable>
void register_cam_query(Callable lambda)
{
  for (ecs::QueryIterator begin = register_cam_query_descr.begin(), end = register_cam_query_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<CameraManager>(0)
    );
  }
}


ecs::SingleQueryDescription set_main_cam_query_descr("set_main_cam_query", {
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
});

template<typename Callable>
void set_main_cam_query(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(set_main_cam_query_descr, eid, begin))
  {
    lambda(
      *begin.get_component<bool>(0),
      *begin.get_component<MainCamera>(1)
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


void arcball_camera_update_func();

ecs::SystemDescription arcball_camera_update_descr("arcball_camera_update", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::EntityId>("arcballCameraTarget"), false}
}, arcball_camera_update_func, ecs::SystemOrder::NO_ORDER, (uint)(ecs::SystemTag::Game));

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
}, freecamera_update_func, ecs::SystemOrder::NO_ORDER, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

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


void update_main_camera_transformations_func();

ecs::SystemDescription update_main_camera_transformations_descr("update_main_camera_transformations", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, update_main_camera_transformations_func, ecs::SystemOrder::RENDER - 5, (uint)(ecs::SystemTag::GameEditor));

void update_main_camera_transformations_func()
{
  for (ecs::QueryIterator begin = update_main_camera_transformations_descr.begin(), end = update_main_camera_transformations_descr.end(); begin != end; ++begin)
  {
    update_main_camera_transformations(
      *begin.get_component<MainCamera>(0)
    );
  }
}


void create_camera_manager_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> create_camera_manager_descr("create_camera_manager", {
  {ecs::get_type_description<CameraManager>("manager"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, create_camera_manager_handler, (uint)(ecs::SystemTag::GameEditor));

void create_camera_manager_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = create_camera_manager_descr.begin(), end = create_camera_manager_descr.end(); begin != end; ++begin)
  {
    create_camera_manager(
      event,
      *begin.get_component<CameraManager>(0),
      *begin.get_component<MainCamera>(1)
    );
  }
}


void set_main_camera_handler(const OnSetMainCamera &event);

ecs::EventDescription<OnSetMainCamera> set_main_camera_descr("set_main_camera", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, set_main_camera_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void set_main_camera_handler(const OnSetMainCamera &event)
{
  for (ecs::QueryIterator begin = set_main_camera_descr.begin(), end = set_main_camera_descr.end(); begin != end; ++begin)
  {
    set_main_camera(
      event,
      *begin.get_component<MainCamera>(0)
    );
  }
}


void set_next_camera_handler(const KeyDownEvent<SDLK_F1> &event);

ecs::EventDescription<KeyDownEvent<SDLK_F1>> set_next_camera_descr("set_next_camera", {
  {ecs::get_type_description<CameraManager>("manager"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, set_next_camera_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void set_next_camera_handler(const KeyDownEvent<SDLK_F1> &event)
{
  for (ecs::QueryIterator begin = set_next_camera_descr.begin(), end = set_next_camera_descr.end(); begin != end; ++begin)
  {
    set_next_camera(
      event,
      *begin.get_component<CameraManager>(0),
      *begin.get_component<MainCamera>(1)
    );
  }
}


void arcball_created_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> arcball_created_descr("arcball_created", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_handler, (uint)(ecs::SystemTag::Game));

void arcball_created_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = arcball_created_descr.begin(), end = arcball_created_descr.end(); begin != end; ++begin)
  {
    arcball_created(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ArcballCamera>(1),
      *begin.get_component<Transform>(2)
    );
  }
}


void arccam_mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> arccam_mouse_move_handler_descr("arccam_mouse_move_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_move_handler_handler, (uint)(ecs::SystemTag::Game));

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


void arccam_mouse_click_handler_handler(const MouseClickEventAnyEvent &event);

ecs::EventDescription<MouseClickEventAnyEvent> arccam_mouse_click_handler_descr("arccam_mouse_click_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_click_handler_handler, (uint)(ecs::SystemTag::Game));

void arccam_mouse_click_handler_handler(const MouseClickEventAnyEvent &event)
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
}, arccam_mouse_wheel_handler_handler, (uint)(ecs::SystemTag::Game));

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
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, freecam_created_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void freecam_created_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = freecam_created_descr.begin(), end = freecam_created_descr.end(); begin != end; ++begin)
  {
    freecam_created(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<FreeCamera>(1),
      *begin.get_component<Transform>(2)
    );
  }
}


void freecam_mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> freecam_mouse_move_handler_descr("freecam_mouse_move_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

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


void freecam_mouse_click_handler_handler(const MouseClickEventAnyEvent &event);

ecs::EventDescription<MouseClickEventAnyEvent> freecam_mouse_click_handler_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void freecam_mouse_click_handler_handler(const MouseClickEventAnyEvent &event)
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


void create_camera_manager_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> create_camera_manager_singl_descr("create_camera_manager", {
  {ecs::get_type_description<CameraManager>("manager"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, create_camera_manager_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void create_camera_manager_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin)
{
  create_camera_manager(
    event,
      *begin.get_component<CameraManager>(0),
      *begin.get_component<MainCamera>(1)
  );
}


void set_main_camera_singl_handler(const OnSetMainCamera &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<OnSetMainCamera> set_main_camera_singl_descr("set_main_camera", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, set_main_camera_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void set_main_camera_singl_handler(const OnSetMainCamera &event, ecs::QueryIterator &begin)
{
  set_main_camera(
    event,
      *begin.get_component<MainCamera>(0)
  );
}


void set_next_camera_singl_handler(const KeyDownEvent<SDLK_F1> &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyDownEvent<SDLK_F1>> set_next_camera_singl_descr("set_next_camera", {
  {ecs::get_type_description<CameraManager>("manager"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, set_next_camera_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void set_next_camera_singl_handler(const KeyDownEvent<SDLK_F1> &event, ecs::QueryIterator &begin)
{
  set_next_camera(
    event,
      *begin.get_component<CameraManager>(0),
      *begin.get_component<MainCamera>(1)
  );
}


void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> arcball_created_singl_descr("arcball_created", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_singl_handler, (uint)(ecs::SystemTag::Game));

void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  arcball_created(
    event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ArcballCamera>(1),
      *begin.get_component<Transform>(2)
  );
}


void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> arccam_mouse_move_handler_singl_descr("arccam_mouse_move_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_move_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void arccam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  arccam_mouse_move_handler(
    event,
      *begin.get_component<ArcballCamera>(0),
      *begin.get_component<bool>(1)
  );
}


void arccam_mouse_click_handler_singl_handler(const MouseClickEventAnyEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseClickEventAnyEvent> arccam_mouse_click_handler_singl_descr("arccam_mouse_click_handler", {
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, arccam_mouse_click_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void arccam_mouse_click_handler_singl_handler(const MouseClickEventAnyEvent &event, ecs::QueryIterator &begin)
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
}, arccam_mouse_wheel_handler_singl_handler, (uint)(ecs::SystemTag::Game));

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
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, freecam_created_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void freecam_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  freecam_created(
    event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<FreeCamera>(1),
      *begin.get_component<Transform>(2)
  );
}


void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> freecam_mouse_move_handler_singl_descr("freecam_mouse_move_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_move_handler(
    event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<bool>(2)
  );
}


void freecam_mouse_click_handler_singl_handler(const MouseClickEventAnyEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseClickEventAnyEvent> freecam_mouse_click_handler_singl_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_singl_handler, (uint)(ecs::SystemTag::Editor|ecs::SystemTag::Game));

void freecam_mouse_click_handler_singl_handler(const MouseClickEventAnyEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_click_handler(
    event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1)
  );
}



