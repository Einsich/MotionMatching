#include "test_recorder.inl"
//Code-generator production

void recorder_ui_func();

ecs::SystemDescription recorder_ui_descr("recorder_ui", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, recorder_ui_func, ecs::SystemOrder::UI, (uint)(ecs::SystemTag::Game));

void recorder_ui_func()
{
  for (ecs::QueryIterator begin = recorder_ui_descr.begin(), end = recorder_ui_descr.end(); begin != end; ++begin)
  {
    recorder_ui(
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_keybord_handler(const KeyEventAnyActionKey &event);

ecs::EventDescription<KeyEventAnyActionKey> listener_keybord_descr("listener_keybord", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_keybord_handler, (uint)(ecs::SystemTag::Game));

void listener_keybord_handler(const KeyEventAnyActionKey &event)
{
  for (ecs::QueryIterator begin = listener_keybord_descr.begin(), end = listener_keybord_descr.end(); begin != end; ++begin)
  {
    listener_keybord(
      event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_mousemove_handler(const MouseMoveEvent &event);

ecs::EventDescription<MouseMoveEvent> listener_mousemove_descr("listener_mousemove", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_mousemove_handler, (uint)(ecs::SystemTag::Game));

void listener_mousemove_handler(const MouseMoveEvent &event)
{
  for (ecs::QueryIterator begin = listener_mousemove_descr.begin(), end = listener_mousemove_descr.end(); begin != end; ++begin)
  {
    listener_mousemove(
      event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
    );
  }
}


void listener_keybord_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<KeyEventAnyActionKey> listener_keybord_singl_descr("listener_keybord", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_keybord_singl_handler, (uint)(ecs::SystemTag::Game));

void listener_keybord_singl_handler(const KeyEventAnyActionKey &event, ecs::QueryIterator &begin)
{
  listener_keybord(
    event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
  );
}


void listener_mousemove_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<MouseMoveEvent> listener_mousemove_singl_descr("listener_mousemove", {
  {ecs::get_type_description<AnimationDataBasePtr>("dataBase"), false},
  {ecs::get_type_description<int>("recordedTest"), false},
  {ecs::get_type_description<int>("recordedState"), false},
  {ecs::get_type_description<float>("recorderStartTime"), false}
}, listener_mousemove_singl_handler, (uint)(ecs::SystemTag::Game));

void listener_mousemove_singl_handler(const MouseMoveEvent &event, ecs::QueryIterator &begin)
{
  listener_mousemove(
    event,
      *begin.get_component<AnimationDataBasePtr>(0),
      *begin.get_component<int>(1),
      *begin.get_component<int>(2),
      *begin.get_component<float>(3)
  );
}



