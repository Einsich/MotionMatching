#include "ecs_camera.inl"
//Code-generator production

ecs::QueryDescription register_cam_query_descr("register_cam_query", {
  {ecs::get_type_description<std::vector<ecs::EntityId>>("sceneCameras"), false}
});

template<typename Callable>
void register_cam_query(Callable lambda)
{
  for (ecs::QueryIterator begin = register_cam_query_descr.begin(), end = register_cam_query_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<std::vector<ecs::EntityId>>(0)
    );
  }
}


ecs::QueryDescription get_main_cam_query_descr("get_main_cam_query", {
  {ecs::get_type_description<ecs::EntityId>("mainCamera"), false}
});

template<typename Callable>
void get_main_cam_query(Callable lambda)
{
  for (ecs::QueryIterator begin = get_main_cam_query_descr.begin(), end = get_main_cam_query_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0)
    );
  }
}


ecs::SingleQueryDescription set_main_cam_query_descr("set_main_cam_query", {
  {ecs::get_type_description<bool>("isMainCamera"), false}
});

template<typename Callable>
void set_main_cam_query(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(set_main_cam_query_descr, eid, begin))
  {
    lambda(
      *begin.get_component<bool>(0)
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


ecs::SingleQueryDescription get_main_cam_property_query_descr("get_main_cam_property_query", {
  {ecs::get_type_description<Camera>("camera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void get_main_cam_property_query(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(get_main_cam_property_query_descr, eid, begin))
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
}, arcball_camera_update_func, ecs::SystemOrder::NO_ORDER);

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


void arccam_update_func();

ecs::SystemDescription arccam_update_descr("arccam_update", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arccam_update_func, ecs::SystemOrder::NO_ORDER);

void arccam_update_func()
{
  for (ecs::QueryIterator begin = arccam_update_descr.begin(), end = arccam_update_descr.end(); begin != end; ++begin)
  {
    arccam_update(
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1),
      *begin.get_component<Transform>(2)
    );
  }
}


void set_main_camera_handler(const OnSetMainCamera &event);

ecs::EventDescription<OnSetMainCamera> set_main_camera_descr("set_main_camera", {
  {ecs::get_type_description<std::vector<ecs::EntityId>>("sceneCameras"), false},
  {ecs::get_type_description<ecs::EntityId>("mainCamera"), false}
}, set_main_camera_handler);

void set_main_camera_handler(const OnSetMainCamera &event)
{
  for (ecs::QueryIterator begin = set_main_camera_descr.begin(), end = set_main_camera_descr.end(); begin != end; ++begin)
  {
    set_main_camera(
      event,
      *begin.get_component<std::vector<ecs::EntityId>>(0),
      *begin.get_component<ecs::EntityId>(1)
    );
  }
}


void set_next_camera_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> set_next_camera_descr("set_next_camera", {
  {ecs::get_type_description<std::vector<ecs::EntityId>>("sceneCameras"), false},
  {ecs::get_type_description<ecs::EntityId>("mainCamera"), false}
}, set_next_camera_handler);

void set_next_camera_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = set_next_camera_descr.begin(), end = set_next_camera_descr.end(); begin != end; ++begin)
  {
    set_next_camera(
      event,
      *begin.get_component<std::vector<ecs::EntityId>>(0),
      *begin.get_component<ecs::EntityId>(1)
    );
  }
}


void arcball_created_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> arcball_created_descr("arcball_created", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_handler);

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
}, arccam_mouse_move_handler_handler);

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
}, arccam_mouse_click_handler_handler);

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
}, arccam_mouse_wheel_handler_handler);

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
}, freecam_created_handler);

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
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_handler);

void freecam_mouse_move_handler_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = freecam_mouse_move_handler_descr.begin(), end = freecam_mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    freecam_mouse_move_handler(
      event,
      *begin.get_component<FreeCamera>(0),
      *begin.get_component<bool>(1)
    );
  }
}


void freecam_mouse_click_handler_handler(const MouseClickEvent &event);

ecs::EventDescription<MouseClickEvent> freecam_mouse_click_handler_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_handler);

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


void set_main_camera_singl_handler(const OnSetMainCamera &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<OnSetMainCamera> set_main_camera_singl_descr("set_main_camera", {
  {ecs::get_type_description<std::vector<ecs::EntityId>>("sceneCameras"), false},
  {ecs::get_type_description<ecs::EntityId>("mainCamera"), false}
}, set_main_camera_singl_handler);

void set_main_camera_singl_handler(const OnSetMainCamera &event, ecs::QueryIterator &begin)
{
  set_main_camera(
    event,
    *begin.get_component<std::vector<ecs::EntityId>>(0),
    *begin.get_component<ecs::EntityId>(1)
  );
}


void set_next_camera_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> set_next_camera_singl_descr("set_next_camera", {
  {ecs::get_type_description<std::vector<ecs::EntityId>>("sceneCameras"), false},
  {ecs::get_type_description<ecs::EntityId>("mainCamera"), false}
}, set_next_camera_singl_handler);

void set_next_camera_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  set_next_camera(
    event,
    *begin.get_component<std::vector<ecs::EntityId>>(0),
    *begin.get_component<ecs::EntityId>(1)
  );
}


void arcball_created_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> arcball_created_singl_descr("arcball_created", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ArcballCamera>("arcballCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, arcball_created_singl_handler);

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
}, arccam_mouse_move_handler_singl_handler);

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
}, arccam_mouse_click_handler_singl_handler);

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
}, arccam_mouse_wheel_handler_singl_handler);

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
}, freecam_created_singl_handler);

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
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_move_handler_singl_handler);

void freecam_mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_move_handler(
    event,
    *begin.get_component<FreeCamera>(0),
    *begin.get_component<bool>(1)
  );
}


void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseClickEvent> freecam_mouse_click_handler_singl_descr("freecam_mouse_click_handler", {
  {ecs::get_type_description<FreeCamera>("freeCamera"), false},
  {ecs::get_type_description<bool>("isMainCamera"), false}
}, freecam_mouse_click_handler_singl_handler);

void freecam_mouse_click_handler_singl_handler(const MouseClickEvent &event, ecs::QueryIterator &begin)
{
  freecam_mouse_click_handler(
    event,
    *begin.get_component<FreeCamera>(0),
    *begin.get_component<bool>(1)
  );
}



