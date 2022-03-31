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
  void load_scene(const string &path);
  void destroy_scene();
  void invalidate_cached_archetype();

  bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return a->order < b->order;
  }
  void SceneManager::start()
  {
    create_all_resources_from_metadata();

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

  void SceneManager::restart_cur_scene()
  {
    ecs::destroy_scene();
/*     for (auto &p : ecs::SingletonTypeInfo::types())
    {
      auto &singleton = p.second;
      singleton.constructor(singleton.getSingleton());        
    } */
    load_scene(currentScene.path);
    send_event(OnSceneCreated());
  }
  void SceneManager::swap_editor_game_scene()
  {
    bool inEditor = core().currentSceneTags & ecs::SystemTag::Game;
    uint32_t tags = core().currentSceneTags;
    tags |= (inEditor ? ecs::SystemTag::Editor : ecs::SystemTag::Game);
    tags &= (!inEditor ? ~ecs::SystemTag::Editor : ~ecs::SystemTag::Game);
    invalidate_cached_archetype();
    start_scene(currentScene.path, tags);
  }
  void SceneManager::start_scene(const string &path, uint newTags)
  {
    currentScene.path = path;
    bool inEditor = newTags & ecs::SystemTag::Editor;
    ecs::EntityContainer &curContainer = inEditor ? currentScene.editorScene : currentScene.gameScene;
    core().currentSceneTags = newTags;
    core().entityContainer = &curContainer;
    core().update_systems_subscribes();
    bool needRestart = (inEditor && !curContainer.loaded) || !inEditor;

    if (needRestart)
    {
      restart_cur_scene();
      curContainer.loaded = true;
    }
  }
  void SceneManager::update_range(const SystemRange &range)
  {
    for (SystemIterator it = range.begin; it != range.end; it++)
      if (Core::allow_system_execute((*it)->tags, core().currentSceneTags))
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
      start_scene(sceneToLoad, core().currentSceneTags);
      sceneToLoad = "";
    }
  }
  
  void SceneManager::destroy_scene()
  {
    process_events();
    core().destroy_all_entities();
    for (Archetype *archetype : currentScene.gameScene.archetypes)
      delete archetype;
    for (Archetype *archetype : currentScene.editorScene.archetypes)
      delete archetype;
    save_all_resources_to_metadata();
  }
}