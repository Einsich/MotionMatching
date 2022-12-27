#include "test_recorder.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache recorder_ui__cache__;

static ecs::QueryCache listener_keybord__cache__;

static ecs::QueryCache listener_mousemove__cache__;

static void recorder_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), recorder_ui__cache__, recorder_ui);
}

static void recorder_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), recorder_ui__cache__, recorder_ui);
}

static void listener_keybord_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const KeyEventAnyActionKey &>(event), listener_keybord__cache__, listener_keybord);
}

static void listener_keybord_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const KeyEventAnyActionKey &>(event), listener_keybord__cache__, listener_keybord);
}

static void listener_mousemove_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const MouseMoveEvent &>(event), listener_mousemove__cache__, listener_mousemove);
}

static void listener_mousemove_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const MouseMoveEvent &>(event), listener_mousemove__cache__, listener_mousemove);
}

static void registration_pull_test_recorder()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "recorder_ui",
  &recorder_ui__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"recordedTest", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()},
    {"recordedState", ecs::get_type_index<int>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<int>()},
    {"recorderStartTime", ecs::get_type_index<float>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  {},
  {},
  {},
  &recorder_ui_handler, &recorder_ui_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "listener_keybord",
  &listener_keybord__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"recordedTest", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"recordedState", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"recorderStartTime", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  {},
  {},
  {},
  &listener_keybord_handler, &listener_keybord_single_handler),
  ecs::EventIndex<KeyEventAnyActionKey>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "listener_mousemove",
  &listener_mousemove__cache__,
  {
    {"tests", ecs::get_type_index<ecs::vector<AnimationTest>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<AnimationTest>>()},
    {"recordedTest", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"recordedState", ecs::get_type_index<int>(), ecs::AccessType::Copy, false, ecs::is_singleton<int>()},
    {"recorderStartTime", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()}
  },
  {},
  {},
  {},
  {},
  {},
  &listener_mousemove_handler, &listener_mousemove_single_handler),
  ecs::EventIndex<MouseMoveEvent>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_test_recorder)
ECS_PULL_DEFINITION(variable_pull_test_recorder)
