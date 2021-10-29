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
}, third_peson_controller_update_func, ecs::SystemOrder::LOGIC, (uint)(ecs::SystemTag::Game));

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
  {ecs::get_type_description<ecs::EntityId>("attachedCamera"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, third_controller_appear_handler, (uint)(ecs::SystemTag::Game));

void third_controller_appear_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = third_controller_appear_descr.begin(), end = third_controller_appear_descr.end(); begin != end; ++begin)
  {
    third_controller_appear(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1)
    );
  }
}


void mouse_move_handler_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> mouse_move_handler_descr("mouse_move_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, mouse_move_handler_handler, (uint)(ecs::SystemTag::Game));

void mouse_move_handler_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = mouse_move_handler_descr.begin(), end = mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    mouse_move_handler(
      event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1),
      *begin.get_component<PersonController>(2),
      *begin.get_component<Settings>(3)
    );
  }
}


void mouse_wheel_handler_handler(const MouseWheelEvent &event);

ecs::EventDescription<MouseWheelEvent> mouse_wheel_handler_descr("mouse_wheel_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, mouse_wheel_handler_handler, (uint)(ecs::SystemTag::Game));

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


void crouch_event_handler_handler(const KeyEventAnyActionKey &event);

ecs::EventDescription<KeyEventAnyActionKey> crouch_event_handler_descr("crouch_event_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, crouch_event_handler_handler, (uint)(ecs::SystemTag::Game));

void crouch_event_handler_handler(const KeyEventAnyActionKey &event)
{
  for (ecs::QueryIterator begin = crouch_event_handler_descr.begin(), end = crouch_event_handler_descr.end(); begin != end; ++begin)
  {
    crouch_event_handler(
      event,
      *begin.get_component<ecs::EntityId>(0)
    );
  }
}


void animation_player_handler_handler(const KeyEventAnyActionKey &event);

ecs::EventDescription<KeyEventAnyActionKey> animation_player_handler_descr("animation_player_handler", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, animation_player_handler_handler, (uint)(ecs::SystemTag::Game));

void animation_player_handler_handler(const KeyEventAnyActionKey &event)
{
  for (ecs::QueryIterator begin = animation_player_handler_descr.begin(), end = animation_player_handler_descr.end(); begin != end; ++begin)
  {
    animation_player_handler(
      event,
      *begin.get_component<AnimationPlayer>(0)
    );
  }
}


void third_controller_appear_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> third_controller_appear_singl_descr("third_controller_appear", {
  {ecs::get_type_description<ecs::EntityId>("attachedCamera"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, third_controller_appear_singl_handler, (uint)(ecs::SystemTag::Game));

void third_controller_appear_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  third_controller_appear(
    event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1)
  );
}


void mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> mouse_move_handler_singl_descr("mouse_move_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, mouse_move_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void mouse_move_handler_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  mouse_move_handler(
    event,
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<ThirdPersonController>(1),
      *begin.get_component<PersonController>(2),
      *begin.get_component<Settings>(3)
  );
}


void mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseWheelEvent> mouse_wheel_handler_singl_descr("mouse_wheel_handler", {
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, mouse_wheel_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void mouse_wheel_handler_singl_handler(const MouseWheelEvent &event, ecs::QueryIterator &begin)
{
  mouse_wheel_handler(
    event,
      *begin.get_component<ThirdPersonController>(0)
  );
}


void crouch_event_handler_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyEventAnyActionKey> crouch_event_handler_singl_descr("crouch_event_handler", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, crouch_event_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void crouch_event_handler_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin)
{
  crouch_event_handler(
    event,
      *begin.get_component<ecs::EntityId>(0)
  );
}


void animation_player_handler_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyEventAnyActionKey> animation_player_handler_singl_descr("animation_player_handler", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<ThirdPersonController>("thirdPersonController"), false}
}, animation_player_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void animation_player_handler_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin)
{
  animation_player_handler(
    event,
      *begin.get_component<AnimationPlayer>(0)
  );
}



