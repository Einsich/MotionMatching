#include "test_recorder.inl"
#include <ecs_perform.h>
//Code-generator production

void recorder_ui_func();

ecs::SystemDescription recorder_ui_descr("recorder_ui", {
  {ecs::get_type_description<vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
recorder_ui_func, ecs::stage::ui, ecs::tags::all, false);

void recorder_ui_func()
{
  ecs::perform_system(recorder_ui_descr, recorder_ui);
}

void listener_keybord_handler(const ecs::Event &event);
void listener_keybord_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription<KeyEventAnyActionKey> listener_keybord_descr("listener_keybord", {
  {ecs::get_type_description<vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
listener_keybord_handler, listener_keybord_singl_handler, ecs::tags::all);

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

ecs::EventDescription<MouseMoveEvent> listener_mousemove_descr("listener_mousemove", {
  {ecs::get_type_description<vector<AnimationTest>>("tests"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, {
}, {},
{},
{},
listener_mousemove_handler, listener_mousemove_singl_handler, ecs::tags::all);

void listener_mousemove_handler(const ecs::Event &event)
{
  ecs::perform_event((const MouseMoveEvent&)event, listener_mousemove_descr, listener_mousemove);
}
void listener_mousemove_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const MouseMoveEvent&)event, listener_mousemove_descr, eid, listener_mousemove);
}


