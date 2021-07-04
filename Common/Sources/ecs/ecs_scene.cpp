#include "ecs_scene.h"
#include "ecs/ecs.h"
#include "manager/entity_container.h"
#include "config.h"
#include "glad/glad.h"
#include "Engine/Profiler/profiler.h"
#include "Engine/imgui/imgui.h"
namespace ecs
{
  bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return a->order < b->order;
  }
  void Scene::start_scene()
  {
    scenes.emplace_back(new SceneEntities());
    scenes.back()->name = "default";
    auto &systems = core().systems;
    std::sort(systems.begin(), systems.end(), system_comparator);
    logic.begin = systems.begin();
    logic.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::EARLY_RENDER;});
    render.begin = logic.end;
    render.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::UI;});
    ui.begin = render.end;
    ui.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::UIMENU;});
    menu.begin = ui.end;
    menu.end = systems.end();

    Input::input().keyboard_event(KeyAction::Down) += createMethodEventHandler(*this, &Scene::keyboard_event_handler);
    Input::input().mouse_click_event() += createMethodEventHandler(*this, &Scene::mouse_click_event_handler);
    Input::input().mouse_move_event() += createMethodEventHandler(*this, &Scene::mouse_move_event_handler);
    Input::input().mouse_wheel_event() += createMethodEventHandler(*this, &Scene::mouse_wheel_event_handler);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  bool Scene::try_start_scene(const string &name, uint tags)
  {
    for (SceneEntities *scene : scenes)
      if (scene->name == name)
      {
        currentScene = scene;
        currentSceneTags = tags;
        core().currentSceneTags = tags;
        core().entityContainer = tags == (uint)ecs::SystemTag::Editor ? &scene->editorScene : &scene->gameScene;
        send_event(OnSceneCreated());
        return true;
      }
    return false;
  }
  
  void create_entities_from_archetypes(const vector<Archetype *> &src);//ecs_core.cpp

  void Scene::swap_editor_game_scene(bool pause)
  {
    uint newTags = currentSceneTags == (uint)ecs::SystemTag::Editor ? (uint)ecs::SystemTag::Game : (uint)ecs::SystemTag::Editor;
    if (currentScene)
    {
      currentSceneTags = newTags;
      core().currentSceneTags = newTags;
      if (newTags == (uint)ecs::SystemTag::Editor)
      {
        currentScene->gamePaused = pause;
        if (!pause)
        {
          core().destroy_entities();
          process_only_events();
          destroy_entities(true);
        }
        core().replace_entity_container(&currentScene->editorScene);
        
      }
      else
      {
        core().replace_entity_container(&currentScene->gameScene);
        if (!currentScene->gamePaused) 
        {       
          create_entities_from_archetypes(currentScene->editorScene.archetypes);
          send_event(OnSceneCreated());
        }
      }
    }
  }
  void Scene::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
      if ((*it)->tags & currentSceneTags)
      {
        //debug_log("execute %s system", (*it)->name.c_str());
        ProfilerLabel label((*it)->name.c_str());
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
    if (ImGui::BeginMainMenuBar())
    {
      update_range(menu);
      ImGui::EndMainMenuBar();
    }

  }
  void Scene::destroy_entities(bool without_copy)
  {
    auto &toDestroy = core().toDestroy;
    for (int i = 0, n = toDestroy.size(); i < n; i++)
    {
      EntityId &eid = toDestroy.front();
      debug_log("destroy %d %d entity", eid.archetype_index(), eid.array_index());
      fflush(stdout);
      core().entityContainer->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), without_copy);
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
    for (SceneEntities *scene: scenes)
    {
      for (Archetype *archetype : scene->gameScene.archetypes)
        delete archetype;
      for (Archetype *archetype : scene->editorScene.archetypes)
        delete archetype;
      delete scene;
    }
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