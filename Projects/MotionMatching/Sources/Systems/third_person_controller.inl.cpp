#include "third_person_controller.inl"
//Code-generator production

ecs::SingleQueryDescription update_attached_camera_descr("update_attached_camera", {
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void update_attached_camera(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(update_attached_camera_descr, eid, begin))
  {
    lambda(
      *begin.get_component<Transform>(0)
    );
  }
}


void third_peson_controller_update_func();

ecs::SystemDescription third_peson_controller_update_descr("third_peson_controller_update", {
  {ecs::get_type_description<ecs::EntityId>("attachedCamera"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, third_peson_controller_update_func, ecs::SystemOrder::LOGIC);

void third_peson_controller_update_func()
{
  for (ecs::QueryIterator begin = third_peson_controller_update_descr.begin(), end = third_peson_controller_update_descr.end(); begin != end; ++begin)
  {
    third_peson_controller_update(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<PersonController>(1),
      *begin.get_component<ThirdPersonController>(2)
    );
  }
}


void third_controller_appear_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> third_controller_appear_descr("third_controller_appear", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, third_controller_appear_handler);

void third_controller_appear_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = third_controller_appear_descr.begin(), end = third_controller_appear_descr.end(); begin != end; ++begin)
  {
    third_controller_appear(
      event,
      *begin.get_component<ThirdPersonController>(0)
    );
  }
}


void mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> mouse_move_handler_descr("mouse_move_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<PersonController>("personController"), false}
}, mouse_move_handler_handler);

void mouse_move_handler_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = mouse_move_handler_descr.begin(), end = mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    mouse_move_handler(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1),
      *begin.get_component<PersonController>(2)
    );
  }
}


void mouse_wheel_handler_handler(const MouseWheelEvent &event);

ecs::EventDescription<MouseWheelEvent> mouse_wheel_handler_descr("mouse_wheel_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, mouse_wheel_handler_handler);

void mouse_wheel_handler_handler(const MouseWheelEvent &event)
{
  for (ecs::QueryIterator begin = mouse_wheel_handler_descr.begin(), end = mouse_wheel_handler_descr.end(); begin != end; ++begin)
  {
    mouse_wheel_handler(
      event,
      *begin.get_component<ThirdPersonController>(0)
    );
  }
}


void view_offset_handler_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> view_offset_handler_descr("view_offset_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, view_offset_handler_handler);

void view_offset_handler_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = view_offset_handler_descr.begin(), end = view_offset_handler_descr.end(); begin != end; ++begin)
  {
    view_offset_handler(
      event,
      *begin.get_component<ThirdPersonController>(0)
    );
  }
}


void crouch_event_handler_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> crouch_event_handler_descr("crouch_event_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, crouch_event_handler_handler);

void crouch_event_handler_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = crouch_event_handler_descr.begin(), end = crouch_event_handler_descr.end(); begin != end; ++begin)
  {
    crouch_event_handler(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1)
    );
  }
}


void animation_player_handler_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> animation_player_handler_descr("animation_player_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, animation_player_handler_handler);

void animation_player_handler_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = animation_player_handler_descr.begin(), end = animation_player_handler_descr.end(); begin != end; ++begin)
  {
    animation_player_handler(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1),
      *begin.get_component<AnimationPlayer>(2)
    );
  }
}


void third_controller_appear_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> third_controller_appear_singl_descr("third_controller_appear", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, third_controller_appear_singl_handler);

void third_controller_appear_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  third_controller_appear(
    event,
    *begin.get_component<ThirdPersonController>(0)
  );
}


void mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> mouse_move_handler_singl_descr("mouse_move_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<PersonController>("personController"), false}
}, mouse_move_handler_singl_handler);

void mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  mouse_move_handler(
    event,
    *begin.get_component<ecs::EntityId>(0),
    *begin.get_component<ThirdPersonController>(1),
    *begin.get_component<PersonController>(2)
  );
}


void mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseWheelEvent> mouse_wheel_handler_singl_descr("mouse_wheel_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, mouse_wheel_handler_singl_handler);

void mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin)
{
  mouse_wheel_handler(
    event,
    *begin.get_component<ThirdPersonController>(0)
  );
}


void view_offset_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> view_offset_handler_singl_descr("view_offset_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, view_offset_handler_singl_handler);

void view_offset_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  view_offset_handler(
    event,
    *begin.get_component<ThirdPersonController>(0)
  );
}


void crouch_event_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> crouch_event_handler_singl_descr("crouch_event_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, crouch_event_handler_singl_handler);

void crouch_event_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  crouch_event_handler(
    event,
    *begin.get_component<ecs::EntityId>(0),
    *begin.get_component<ThirdPersonController>(1)
  );
}


void animation_player_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> animation_player_handler_singl_descr("animation_player_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, animation_player_handler_singl_handler);

void animation_player_handler_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  animation_player_handler(
    event,
    *begin.get_component<ecs::EntityId>(0),
    *begin.get_component<ThirdPersonController>(1),
    *begin.get_component<AnimationPlayer>(2)
  );
}



