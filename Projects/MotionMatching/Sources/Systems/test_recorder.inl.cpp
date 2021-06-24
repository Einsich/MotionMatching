#include "test_recorder.inl"
//Code-generator production

void recorder_ui_func();

ecs::SystemDescription recorder_ui_descr("recorder_ui", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, recorder_ui_func, ecs::SystemOrder::UI);

void recorder_ui_func()
{
  for (ecs::QueryIterator begin = recorder_ui_descr.begin(), end = recorder_ui_descr.end(); begin != end; ++begin)
  {
    recorder_ui(
      *begin.get_component<MotionMatchingScene>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_keybord_handler(const KeyboardEvent &event);

ecs::EventDescription<KeyboardEvent> listener_keybord_descr("listener_keybord", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_keybord_handler);

void listener_keybord_handler(const KeyboardEvent &event)
{
  for (ecs::QueryIterator begin = listener_keybord_descr.begin(), end = listener_keybord_descr.end(); begin != end; ++begin)
  {
    listener_keybord(
      event,
      *begin.get_component<MotionMatchingScene>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_mousemove_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> listener_mousemove_descr("listener_mousemove", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_mousemove_handler);

void listener_mousemove_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = listener_mousemove_descr.begin(), end = listener_mousemove_descr.end(); begin != end; ++begin)
  {
    listener_mousemove(
      event,
      *begin.get_component<MotionMatchingScene>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_keybord_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyboardEvent> listener_keybord_singl_descr("listener_keybord", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_keybord_singl_handler);

void listener_keybord_singl_handler(const KeyboardEvent &event, ecs::QueryIterator &begin)
{
  listener_keybord(
    event,
      *begin.get_component<MotionMatchingScene>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
  );
}


void listener_mousemove_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> listener_mousemove_singl_descr("listener_mousemove", {
  {ecs::get_type_description<MotionMatchingScene>("motionMatchingScene"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_mousemove_singl_handler);

void listener_mousemove_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  listener_mousemove(
    event,
      *begin.get_component<MotionMatchingScene>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
  );
}



