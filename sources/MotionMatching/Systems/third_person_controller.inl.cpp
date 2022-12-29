#include "third_person_controller.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache update_attached_camera__cache__;

static ecs::QueryCache third_peson_controller_update__cache__;

static ecs::QueryCache third_controller_appear__cache__;

static ecs::QueryCache mouse_move_handler__cache__;

static ecs::QueryCache mouse_wheel_handler__cache__;

static ecs::QueryCache crouch_event_handler__cache__;

static ecs::QueryCache animation_player_handler__cache__;

template<typename Callable>
static void update_attached_camera(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Transform&>(update_attached_camera__cache__, eid, lambda);
}

static void third_peson_controller_update_implementation()
{
  ecs::perform_system(third_peson_controller_update__cache__, third_peson_controller_update);
}

static void third_controller_appear_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), third_controller_appear__cache__, third_controller_appear);
}

static void third_controller_appear_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), third_controller_appear__cache__, third_controller_appear);
}

static void mouse_move_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), mouse_move_handler__cache__, mouse_move_handler);
}

static void mouse_move_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), mouse_move_handler__cache__, mouse_move_handler);
}

static void mouse_wheel_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseWheelEvent &>(event), mouse_wheel_handler__cache__, mouse_wheel_handler);
}

static void mouse_wheel_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseWheelEvent &>(event), mouse_wheel_handler__cache__, mouse_wheel_handler);
}

static void crouch_event_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyEventAnyActionKey &>(event), crouch_event_handler__cache__, crouch_event_handler);
}

static void crouch_event_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyEventAnyActionKey &>(event), crouch_event_handler__cache__, crouch_event_handler);
}

static void animation_player_handler_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyEventAnyActionKey &>(event), animation_player_handler__cache__, animation_player_handler);
}

static void animation_player_handler_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyEventAnyActionKey &>(event), animation_player_handler__cache__, animation_player_handler);
}

static void registration_pull_third_person_controller()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "update_attached_camera",
  &update_attached_camera__cache__,
  {
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Transform>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "",
  "third_peson_controller_update",
  &third_peson_controller_update__cache__,
  {
    {"attachedCamera", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()},
    {"thirdPersonController", ecs::get_type_index<ThirdPersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ThirdPersonController>()}
  },
  {},
  {},
  "act",
  {},
  {},
  {},
  &third_peson_controller_update_implementation));

  ecs::register_event(ecs::EventDescription(
  "",
  "third_controller_appear",
  &third_controller_appear__cache__,
  {
    {"attachedCamera", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::EntityId>()},
    {"thirdPersonController", ecs::get_type_index<ThirdPersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ThirdPersonController>()}
  },
  {},
  {},
  {},
  {},
  {},
  &third_controller_appear_handler, &third_controller_appear_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "mouse_move_handler",
  &mouse_move_handler__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()},
    {"thirdPersonController", ecs::get_type_index<ThirdPersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ThirdPersonController>()},
    {"personController", ecs::get_type_index<PersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<PersonController>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  {},
  {},
  {},
  &mouse_move_handler_handler, &mouse_move_handler_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "mouse_wheel_handler",
  &mouse_wheel_handler__cache__,
  {
    {"thirdPersonController", ecs::get_type_index<ThirdPersonController>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ThirdPersonController>()}
  },
  {},
  {},
  {},
  {},
  {},
  &mouse_wheel_handler_handler, &mouse_wheel_handler_single_handler),
  ecs::EventIndex<MouseWheelEvent>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "crouch_event_handler",
  &crouch_event_handler__cache__,
  {
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::Copy, false, ecs::is_singleton<ecs::EntityId>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  {},
  {},
  {},
  &crouch_event_handler_handler, &crouch_event_handler_single_handler),
  ecs::EventIndex<KeyEventAnyActionKey>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "animation_player_handler",
  &animation_player_handler__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<AnimationPlayer>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  {},
  {},
  {},
  &animation_player_handler_handler, &animation_player_handler_single_handler),
  ecs::EventIndex<KeyEventAnyActionKey>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_third_person_controller)
ECS_PULL_DEFINITION(variable_pull_third_person_controller)
