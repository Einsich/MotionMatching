#include "person_controller.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache peson_controller_update__cache__;

static ecs::QueryCache controller_mouse_move_handler__cache__;

static ecs::QueryCache controller_crouch_event_handler__cache__;

static void peson_controller_update_implementation()
{
  ecs::perform_system(peson_controller_update__cache__, peson_controller_update);
}

static void controller_mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ControllerMouseMoveEvent &>(event), controller_mouse_move_handler__cache__, controller_mouse_move_handler);
}

static void controller_mouse_move_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ControllerMouseMoveEvent &>(event), controller_mouse_move_handler__cache__, controller_mouse_move_handler);
}

static void controller_crouch_event_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ControllerKeyBoardEvent &>(event), controller_crouch_event_handler__cache__, controller_crouch_event_handler);
}

static void controller_crouch_event_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ControllerKeyBoardEvent &>(event), controller_crouch_event_handler__cache__, controller_crouch_event_handler);
}

static void registration_pull_person_controller()
{
  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/person_controller.inl:94",
  "peson_controller_update",
  &peson_controller_update__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()},
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()},
    {"animationTester", ecs::get_type_index<AnimationTester>(), ecs::AccessType::ReadWrite, true, ecs::is_singleton<AnimationTester>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()},
    {"controllerIndex", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, true, ecs::is_singleton<int>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SettingsContainer>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {"game"},
  &peson_controller_update_implementation));

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/person_controller.inl:221",
  "controller_mouse_move_handler",
  &controller_mouse_move_handler__cache__,
  {
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  {},
  {},
  {},
  &controller_mouse_move_handler_handler, &controller_mouse_move_handler_single_handler),
  ecs::EventIndex<ControllerMouseMoveEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/MotionMatching/Systems/person_controller.inl:233",
  "controller_crouch_event_handler",
  &controller_crouch_event_handler__cache__,
  {
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()}
  },
  {},
  {},
  {},
  {},
  {},
  &controller_crouch_event_handler_handler, &controller_crouch_event_handler_single_handler),
  ecs::EventIndex<ControllerKeyBoardEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_person_controller)
ECS_PULL_DEFINITION(variable_pull_person_controller)
