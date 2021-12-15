#include "ecs_scene.h"
#include "ecs.h"
#include "manager/entity_container.h"
#include "glad/glad.h"
#include "profiler/profiler.h"
#include "imgui.h"

void create_all_resources_from_metadata();
void save_all_resources_to_metadata();
namespace ecs
{
  bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return a->order < b->order;
  }
  void SceneManager::start(bool editor)
  {
    create_all_resources_from_metadata();
    if (editor)
    {
      scenes.emplace_back(new Scene{"default", "", {}, {}, true, false});
      scenes.back()->editorScene.dontAddable = scenes.back()->gameScene.dontAddable = true;
      scenes.back()->editorScene.loaded = scenes.back()->gameScene.loaded = true;
    }

    auto &systems = core().systems;
    std::sort(systems.begin(), systems.end(), system_comparator);
    logic.begin = systems.begin();
    logic.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::RENDER;});
    render.begin = logic.end;
    render.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::UI;});
    ui.begin = render.end;
    ui.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->order >= (int)SystemOrder::UIMENU;});
    menu.begin = ui.end;
    menu.end = systems.end();


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  void save(std::ostream& os, const std::vector<Archetype*> &archetypes);
  void load(std::istream& is, std::vector<Archetype*> &archetypes, EntityPull &entityPull);

  static void load_scene(const Scene &scene, ecs::EntityContainer &scene_container)
  {
    std::ifstream file(scene.path, ios::binary);
    load(file, core().entityContainer->archetypes, core().entityContainer->entityPull);
    scene_container.loaded = true;
  }
  bool SceneManager::try_start_scene(const string &name, uint tags)
  {
    for (Scene *scene : scenes)
      if (scene->name == name)
      {
        currentScene = scene;
        currentSceneTags = tags;
        bool editor = tags & ecs::SystemTag::Editor;
        core().currentSceneTags = tags;
        ecs::EntityContainer &curContainer = editor ? scene->editorScene : scene->gameScene;
        core().entityContainer = &curContainer;
        if (!curContainer.loaded)
          load_scene(*currentScene, curContainer);
        send_event(OnSceneCreated());
        return true;
      }
    return false;
  }
  
  void create_entities_from_archetypes(const vector<Archetype *> &src);//ecs_core.cpp

  void SceneManager::swap_editor_game_scene(bool pause)
  {
    uint newTags = currentSceneTags == (uint)ecs::SystemTag::Editor ? (uint)ecs::SystemTag::Game : (uint)ecs::SystemTag::Editor;
    if (currentScene)
    {
      for (auto &p : ecs::SingletonTypeInfo::types())
      {
        auto &singleton = p.second;
        singleton.constructor(singleton.getSingleton());        
      }
      currentScene->inEditor = newTags == (uint)ecs::SystemTag::Editor;
      ecs::EntityContainer &curContainer = currentScene->inEditor ? currentScene->editorScene : currentScene->gameScene;
      if (currentScene->inEditor)
      {
        currentScene->gamePaused = pause;
        if (!pause)
        {
          process_events();
          core().destroy_all_entities();
        }
        currentSceneTags = newTags;
        core().currentSceneTags = newTags;
        if (curContainer.loaded)
        {
          core().replace_entity_container(&curContainer);
        }
        else
        {
          core().entityContainer = &curContainer;
          load_scene(*currentScene, curContainer);
          send_event(OnSceneCreated());
        }
      }
      else
      {
        currentSceneTags = newTags;
        core().currentSceneTags = newTags;
        core().replace_entity_container(&curContainer);
        if (!currentScene->gamePaused) 
        {       
          create_entities_from_archetypes(currentScene->editorScene.archetypes);
          send_event(OnSceneCreated());
        }
      }
    }
  }
  void SceneManager::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
      if (Core::allow_system_execute((*it)->tags, currentSceneTags))
      {
        ProfilerLabel label((*it)->name.c_str());
        (*it)->execute();
      }
  }
  void SceneManager::update_logic()
  {
    update_range(logic);
  }
  void SceneManager::update_render()
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_range(render);
    glFlush();
  }
  void SceneManager::update_ui()
  {
    update_range(ui);
    if (ImGui::BeginMainMenuBar())
    {
      update_range(menu);
      ImGui::EndMainMenuBar();
    }

  }

  void load_scene(const string &path);
  void destroy_scene();
  void SceneManager::process_events()
  {
    auto &events = core().events;
    for (int i = 0, n = events.size(); i < n; i++)
    {
      events.front()();
      events.pop();
    }
    core().destroy_entities_from_destroy_queue(true);
    string &sceneToLoad = core().sceneToLoad;
    bool &reloadScene = core().reloadScene;
    if (sceneToLoad != "")
    {
      if (reloadScene)
        ecs::destroy_scene();
      load_scene(sceneToLoad);
      sceneToLoad = "";
      reloadScene = false;
    }
  }
  
  void SceneManager::save_current_scene()
  {
    #ifndef RELEASE
    if (currentScene && filesystem::exists(currentScene->path) && currentScene->editorScene.loaded)
    {
      //ofstream file (currentScene->path, ios::binary);
      //save(file, currentScene->editorScene.archetypes);
    }
    #endif
  }
  void SceneManager::destroy_scene()
  {
    save_current_scene();
    process_events();
    core().destroy_all_entities();
    for (Scene *scene: scenes)
    {
      for (Archetype *archetype : scene->gameScene.archetypes)
        delete archetype;
      for (Archetype *archetype : scene->editorScene.archetypes)
        delete archetype;
      delete scene;
    }
    save_all_resources_to_metadata();
  }
  
  void SceneManager::add_open_scene(const filesystem::path &path, bool need_to_add, bool need_to_open)
  {
    if (need_to_add)
    {
      scenes.emplace_back(new Scene{path.stem().string(), path.string(), {}, {}, false, false});
    }
    if (need_to_open)
    {
      save_current_scene();
      try_start_scene(path.stem().string(), (uint)ecs::SystemTag::Editor);
    }
  }
}