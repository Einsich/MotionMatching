#include "ecs_scene.h"
#include "ecs/ecs.h"
#include "Application/config.h"
#include "glad/glad.h"
namespace ecs
{
  bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return a->order < b->order;
  }
  void Scene::start_scene()
  {
    get_type_description<EntityId>("eid");
    get_type_description<std::string>("project");
    auto &systems = core().systems;
    std::sort(systems.begin(), systems.end(), system_comparator);
    logic.begin = systems.begin();
    logic.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::EARLY_RENDER;});
    render.begin = logic.end;
    render.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::UI;});
    ui.begin = render.end;
    ui.end = systems.end();

    {
      ecs::ComponentInitializerList list;
      list.add<std::string>("project") = std::string(get_config("project"));
      ecs::create_entity(list);
    }
    Input::input().keyboard_event(KeyAction::Down) += createMethodEventHandler(*this, &Scene::keyboard_event_handler);
    Input::input().mouse_click_event() += createMethodEventHandler(*this, &Scene::mouse_click_event_handler);
    Input::input().mouse_move_event() += createMethodEventHandler(*this, &Scene::mouse_move_event_handler);
    Input::input().mouse_wheel_event() += createMethodEventHandler(*this, &Scene::mouse_wheel_event_handler);
    send_event(OnSceneCreated());

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
  void Scene::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
    {
      //debug_log("execute %s system", (*it)->name.c_str());
      (*it)->execute();
    }
  }
  void Scene::update_logic()
  {
    update_range(logic);
  }
  void Scene::update_render()
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_range(render);
    glFlush();
  }
  void Scene::update_ui()
  {
    update_range(ui);

  }
  void Scene::destroy_entities(bool without_copy)
  {
    auto &toDestroy = core().toDestroy;
    for (int i = 0, n = toDestroy.size(); i < n; i++)
    {
      EntityId &eid = toDestroy.front();
      core().archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), without_copy);
      toDestroy.pop();
    }
  }
  void Scene::process_only_events()
  {
    auto &events = core().events;
    for (int i = 0, n = events.size(); i < n; i++)
    {
      events.front()();
      events.pop();
    }
  }
  void Scene::process_events()
  {
    //debug_log("process events");
    process_only_events();
    destroy_entities(false);
  }
  void Scene::destroy_scene()
  {
    core().destroy_entities();
    process_only_events();
    destroy_entities(true);
    core().~Core();
  }
  void Scene::keyboard_event_handler(const KeyboardEvent &event)
  {
    ecs::send_event(event);
  }
  void Scene::mouse_click_event_handler(const MouseClickEvent &event)
  {
    ecs::send_event(event);
  }
  void Scene::mouse_move_event_handler(const MouseMoveEvent &event)
  {
    ecs::send_event(event);
  }
  void Scene::mouse_wheel_event_handler(const MouseWheelEvent &event)
  {
    ecs::send_event(event);
  }
}