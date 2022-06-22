#include "ecs_scene.h"
#include "ecs.h"
#include "ecs_event_impl.h"
#include "manager/entity_container.h"
#include "glad/glad.h"
#include "profiler/profiler.h"
#include "imgui.h"

void create_all_resources_from_metadata();
void save_all_resources_to_metadata();
namespace ecs
{
  void load_scene(const DataBlock &scene);
  void destroy_scene();
  void invalidate_cached_archetype();


  void SceneManager::start()
  {
    create_all_resources_from_metadata();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

  }
  void SceneManager::sort_systems()
  {
    act = ecs::get_system_stage("act");
    before_render = ecs::get_system_stage("before_render");
    render = ecs::get_system_stage("render");
    ui = ecs::get_system_stage("ui");
    menu = ecs::get_system_stage("ui_menu");
  }

  void save(std::ostream& os, const ecs::vector<Archetype*> &archetypes);
  void load(std::istream& is, ecs::vector<Archetype*> &archetypes, EntityPull &entityPull);

  void SceneManager::restart_cur_scene()
  {
    ecs::destroy_scene();
/*     for (auto &p : ecs::SingletonTypeInfo::types())
    {
      auto &singleton = p.second;
      singleton.constructor(singleton.getSingleton());        
    } */
    load_scene(currentScene.rawScene);
    send_event(OnSceneCreated());
  }
  void SceneManager::swap_editor_game_scene()
  {
    invalidate_cached_archetype();
    currentScene.inEditor = !currentScene.inEditor;
    init_scene();
  }
  void SceneManager::start_scene(const std::string &path, bool inEditor)
  {
    currentScene.rawScene = DataBlock(path);
    currentScene.path = path.c_str();
    currentScene.inEditor = inEditor;
    init_scene();
  }
  void SceneManager::init_scene()
  {
    bool inEditor = currentScene.inEditor;
    ecs::EntityContainer &curContainer = inEditor ? currentScene.editorScene : currentScene.gameScene;
    core().currentSceneTags = inEditor ? "editor" : "game";
    core().entityContainer = &curContainer;
    core().resolve_system_order_and_subscribes();
    sort_systems();
    core().update_systems_subscribes();
    bool needRestart = (inEditor && !curContainer.loaded) || !inEditor;

    if (needRestart)
    {
      restart_cur_scene();
      curContainer.loaded = true;
    }
  }
  static void update_range(const SystemStageInterval &range)
  {
    for (auto system = range.begin; system < range.end; ++system)
    {
      ProfilerLabel label((*system)->name.c_str());
      (*system)->execute();
    }
  }
  void SceneManager::update_act()
  {
    update_range(act);
  }
  void SceneManager::update_render()
  {
    update_range(before_render);
    ProfilerLabelGPU label("ecs_render");
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
    if (sceneToLoad != "")
    {
      start_scene(sceneToLoad.c_str(), currentScene.inEditor);
      sceneToLoad = "";
    }
  }
  
  void SceneManager::destroy_scene()
  {
    process_events();
    core().destroy_all_entities();

    save_all_resources_to_metadata();
  }
}