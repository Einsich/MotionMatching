#include "ecs_scene.h"
#include "ecs/ecs.h"
#include "manager/entity_container.h"
#include "config.h"
#include "glad/glad.h"
#include "Engine/Profiler/profiler.h"
#include "Engine/imgui/imgui.h"

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
      scenes.emplace_back(new Scene{"default", "", {}, {}, true, true, false});
      scenes.back()->editorScene.dontAddable = scenes.back()->gameScene.dontAddable = true;
    }

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


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  void save(std::ostream& os, const std::vector<Archetype*> &archetypes);
  void load(std::istream& is, std::vector<Archetype*> &archetypes, EntityPull &entityPull);

  bool SceneManager::try_start_scene(const string &name, uint tags)
  {
    for (Scene *scene : scenes)
      if (scene->name == name)
      {
        currentScene = scene;
        currentSceneTags = tags;
        bool editor = tags == (uint)ecs::SystemTag::Editor;
        core().currentSceneTags = tags;
        core().entityContainer = editor ? &scene->editorScene : &scene->gameScene;
        if (!currentScene->loaded)
        {
          std::ifstream file(currentScene->path, ios::binary);
          load(file, core().entityContainer->archetypes, core().entityContainer->entityPull);
          currentScene->loaded = true;
        }
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
      currentSceneTags = newTags;
      core().currentSceneTags = newTags;
      for (auto &p : ecs::SingletonTypeInfo::types())
      {
        auto &singleton = p.second;
        singleton.constructor(singleton.getSingleton());        
      }
      currentScene->inEditor = newTags == (uint)ecs::SystemTag::Editor;
      if (currentScene->inEditor)
      {
        currentScene->gamePaused = pause;
        if (!pause)
        {
          core().destroy_entities();
          process_only_events();
          destroy_entities(false);
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
  void SceneManager::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
      if ((*it)->tags & currentSceneTags)
      {
        //debug_log("execute %s system", (*it)->name.c_str());
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
  void SceneManager::destroy_entities(bool with_swap)
  {
    auto &toDestroy = core().toDestroy;
    for (int i = 0, n = toDestroy.size(); i < n; i++)
    {
      EntityId &eid = toDestroy.front();
      debug_log("destroy %d %d entity", eid.archetype_index(), eid.array_index());
      fflush(stdout);
      core().entityContainer->archetypes[eid.archetype_index()]->destroy_entity(eid.array_index(), with_swap);
      core().entityContainer->entityPull.destroy_entity(eid);
      toDestroy.pop();
      
    }
  }
  void SceneManager::process_only_events()
  {
    auto &events = core().events;
    for (int i = 0, n = events.size(); i < n; i++)
    {
      events.front()();
      events.pop();
    }
  }
  void SceneManager::process_events()
  {
    //debug_log("process events");
    process_only_events();
    destroy_entities(true);
  }
  
  void SceneManager::save_current_scene()
  {
    #ifndef RELEASE
    if (currentScene && filesystem::exists(currentScene->path))
    {
      ofstream file (currentScene->path, ios::binary);
      save(file, currentScene->editorScene.archetypes);
    }
    #endif
  }
  void SceneManager::destroy_scene()
  {
    save_current_scene();
    core().destroy_entities();
    process_only_events();
    destroy_entities(false);
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
      scenes.emplace_back(new Scene{path.stem().string(), path.string(), {}, {}, false, false, false});
    }
    if (need_to_open)
    {
      save_current_scene();
      try_start_scene(path.stem().string(), (uint)ecs::SystemTag::Editor);
    }
  }
}