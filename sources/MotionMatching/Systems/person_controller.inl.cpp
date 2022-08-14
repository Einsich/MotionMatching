#include "person_controller.inl"
#include <ecs_perform.h>
//Code-generator production

void peson_controller_update_func();

ecs::SystemDescription peson_controller_update_descr("peson_controller_update", {
  {ecs::get_type_hash<AnimationPlayer>(), ecs::get_name_hash("animationPlayer"), false},
  {ecs::get_type_hash<PersonController>(), ecs::get_name_hash("personController"), false},
  {ecs::get_type_hash<AnimationTester>(), ecs::get_name_hash("animationTester"), true},
  {ecs::get_type_hash<Transform>(), ecs::get_name_hash("transform"), false},
  {ecs::get_type_hash<int>(), ecs::get_name_hash("controllerIndex"), true},
  {ecs::get_type_hash<SettingsContainer>(), ecs::get_name_hash("settingsContainer"), false}
}, {
},
{},
{},
peson_controller_update_func, "act", {"game"}, false);

void peson_controller_update_func()
{
  ecs::perform_system(peson_controller_update_descr, peson_controller_update);
}

void controller_mouse_move_handler_handler(const ecs::Event &event);
void controller_mouse_move_handler_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription controller_mouse_move_handler_descr(
  ecs::get_mutable_event_handlers<ControllerMouseMoveEvent>(), "controller_mouse_move_handler", {
  {ecs::get_type_hash<PersonController>(), ecs::get_name_hash("personController"), false},
  {ecs::get_type_hash<Settings>(), ecs::get_name_hash("settings"), false}
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
  {ecs::get_type_hash<PersonController>(), ecs::get_name_hash("personController"), false}
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


