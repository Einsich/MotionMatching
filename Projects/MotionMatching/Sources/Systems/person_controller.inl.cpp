#include "person_controller.inl"
//Code-generator production

void peson_controller_update_func();

ecs::SystemDescription peson_controller_update_descr("peson_controller_update", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<AnimationTester>("animationTester"), true},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<int>("controllerIndex"), true},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, peson_controller_update_func, ecs::SystemOrder::LOGIC, (uint)(ecs::SystemTag::Game));

void peson_controller_update_func()
{
  for (ecs::QueryIterator begin = peson_controller_update_descr.begin(), end = peson_controller_update_descr.end(); begin != end; ++begin)
  {
    peson_controller_update(
      *begin.get_component<AnimationPlayer>(0),
      *begin.get_component<PersonController>(1),
       begin.get_component<AnimationTester>(2),
      *begin.get_component<Transform>(3),
       begin.get_component<int>(4),
      *begin.get_component<SettingsContainer>(5)
    );
  }
}


void controller_mouse_move_handler_handler(const ControllerMouseMoveEvent &event);

ecs::EventDescription<ControllerMouseMoveEvent> controller_mouse_move_handler_descr("controller_mouse_move_handler", {
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, controller_mouse_move_handler_handler, (uint)(ecs::SystemTag::Game));

void controller_mouse_move_handler_handler(const ControllerMouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = controller_mouse_move_handler_descr.begin(), end = controller_mouse_move_handler_descr.end(); begin != end; ++begin)
  {
    controller_mouse_move_handler(
      event,
      *begin.get_component<PersonController>(0),
      *begin.get_component<Settings>(1)
    );
  }
}


void controller_crouch_event_handler_handler(const ControllerKeyBoardEvent &event);

ecs::EventDescription<ControllerKeyBoardEvent> controller_crouch_event_handler_descr("controller_crouch_event_handler", {
  {ecs::get_type_description<PersonController>("personController"), false}
}, controller_crouch_event_handler_handler, (uint)(ecs::SystemTag::Game));

void controller_crouch_event_handler_handler(const ControllerKeyBoardEvent &event)
{
  for (ecs::QueryIterator begin = controller_crouch_event_handler_descr.begin(), end = controller_crouch_event_handler_descr.end(); begin != end; ++begin)
  {
    controller_crouch_event_handler(
      event,
      *begin.get_component<PersonController>(0)
    );
  }
}


void controller_mouse_move_handler_singl_handler(const ControllerMouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ControllerMouseMoveEvent> controller_mouse_move_handler_singl_descr("controller_mouse_move_handler", {
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, controller_mouse_move_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void controller_mouse_move_handler_singl_handler(const ControllerMouseMoveEvent &event, ecs::QueryIterator &begin)
{
  controller_mouse_move_handler(
    event,
      *begin.get_component<PersonController>(0),
      *begin.get_component<Settings>(1)
  );
}


void controller_crouch_event_handler_singl_handler(const ControllerKeyBoardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ControllerKeyBoardEvent> controller_crouch_event_handler_singl_descr("controller_crouch_event_handler", {
  {ecs::get_type_description<PersonController>("personController"), false}
}, controller_crouch_event_handler_singl_handler, (uint)(ecs::SystemTag::Game));

void controller_crouch_event_handler_singl_handler(const ControllerKeyBoardEvent &event, ecs::QueryIterator &begin)
{
  controller_crouch_event_handler(
    event,
      *begin.get_component<PersonController>(0)
  );
}



