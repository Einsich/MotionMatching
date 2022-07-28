#include "person_controller.inl"
#include <ecs_perform.h>
//Code-generator production

void peson_controller_update_func();

ecs::SystemDescription peson_controller_update_descr("peson_controller_update", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<AnimationTester>("animationTester"), true},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<int>("controllerIndex"), true},
  {ecs::get_type_description<SettingsContainer>("settingsContainer"), false}
}, {
},
{},
{},
peson_controller_update_func, "act", {}, false);

void peson_controller_update_func()
{
  ecs::perform_system(peson_controller_update_descr, peson_controller_update);
}

void controller_mouse_move_handler_handler(const ecs::Event &event);
void controller_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription controller_mouse_move_handler_descr(
  ecs::get_mutable_event_handlers<ControllerMouseMoveEvent>(), "controller_mouse_move_handler", {
  {ecs::get_type_description<PersonController>("personController"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, {
},
{},
{},
controller_mouse_move_handler_handler, controller_mouse_move_handler_singl_handler, {});

void controller_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const ControllerMouseMoveEvent&)event, controller_mouse_move_handler_descr, controller_mouse_move_handler);
}
void controller_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ControllerMouseMoveEvent&)event, controller_mouse_move_handler_descr, eid, controller_mouse_move_handler);
}

void controller_crouch_event_handler_handler(const ecs::Event &event);
void controller_crouch_event_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription controller_crouch_event_handler_descr(
  ecs::get_mutable_event_handlers<ControllerKeyBoardEvent>(), "controller_crouch_event_handler", {
  {ecs::get_type_description<PersonController>("personController"), false}
}, {
},
{},
{},
controller_crouch_event_handler_handler, controller_crouch_event_handler_singl_handler, {});

void controller_crouch_event_handler_handler(const ecs::Event &event)
{
  ecs::perform_event((const ControllerKeyBoardEvent&)event, controller_crouch_event_handler_descr, controller_crouch_event_handler);
}
void controller_crouch_event_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ControllerKeyBoardEvent&)event, controller_crouch_event_handler_descr, eid, controller_crouch_event_handler);
}


