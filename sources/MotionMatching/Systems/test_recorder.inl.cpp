#include "test_recorder.inl"
#include <ecs_perform.h>
//Code-generator production

void recorder_ui_func();

ecs::SystemDescription recorder_ui_descr("recorder_ui", {
  {ecs::get_type_description<ecs::vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
recorder_ui_func, "ui", {}, false);

void recorder_ui_func()
{
  ecs::perform_system(recorder_ui_descr, recorder_ui);
}

void listener_keybord_handler(const ecs::Event &event);
void listener_keybord_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription listener_keybord_descr(
  ecs::get_mutable_event_handlers<KeyEventAnyActionKey>(), "listener_keybord", {
  {ecs::get_type_description<ecs::vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
listener_keybord_handler, listener_keybord_singl_handler, {});

void listener_keybord_handler(const ecs::Event &event)
{
  ecs::perform_event((const KeyEventAnyActionKey&)event, listener_keybord_descr, listener_keybord);
}
void listener_keybord_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const KeyEventAnyActionKey&)event, listener_keybord_descr, eid, listener_keybord);
}

void listener_mousemove_handler(const ecs::Event &event);
void listener_mousemove_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription listener_mousemove_descr(
  ecs::get_mutable_event_handlers<MouseMoveEvent>(), "listener_mousemove", {
  {ecs::get_type_description<ecs::vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
listener_mousemove_handler, listener_mousemove_singl_handler, {});

void listener_mousemove_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, listener_mousemove_descr, listener_mousemove);
}
void listener_mousemove_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, listener_mousemove_descr, eid, listener_mousemove);
}


