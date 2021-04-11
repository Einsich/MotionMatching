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
    send_event(OnSceneCreated());

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
  void Scene::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
    {
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
  void Scene::process_events()
  {
    auto &events = core().events;
    for (int i = 0, n = events.size(); i < n; i++)
    {
      events.front()();
      events.pop();
    }
  }
  void Scene::destroy_scene()
  {
    core().~Core();
  }
}