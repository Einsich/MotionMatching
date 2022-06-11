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

  static bool system_comparator(const SystemDescription *a, const SystemDescription *b)
  {
    return a->stage < b->stage;
  }
  static void dfs(uint v, const vector<vector<uint>> &edges, vector<bool> &used, vector<uint> &answer)
  {
    used[v] = true;
    for (uint to : edges[v])
    {
      if (!used[to])
        dfs(to, edges, used, answer);
    }
    answer.push_back(v);
  }
  template<typename Description>
  static void topological_sort(std::vector<Description *> &systems)
  {
    vector<vector<uint>> edge(systems.size());
    vector<bool> used(systems.size(), false);
    vector<uint> answer;
    answer.reserve(systems.size());
    map<string, int> nameMap;
    for (uint i = 0; i < systems.size(); i++)
      if (!systems[i]->isQuery)
        nameMap[systems[i]->name] = i;
    for (uint i = 0; i < systems.size(); i++)
    {
      for (const string &before : systems[i]->before)
      {
        auto it = nameMap.find(before);
        if (it != nameMap.end())
        {
          edge[it->second].push_back(i);
        }
        else
        {
          debug_error("%s didn't exist for before %s", before.c_str(), systems[i]->name.c_str());
        }
      }
      for (const string &after : systems[i]->after)
      {
        auto it = nameMap.find(after);
        if (it != nameMap.end())
        {
          edge[i].push_back(it->second);
        }
        else
        {
          debug_error("%s didn't exist for after %s", after.c_str(), systems[i]->name.c_str());
        }
      }
    }
    for (uint i = 0; i < systems.size(); i++)
    {
      if (!used[i])
        dfs(i, edge, used, answer);
    }
    std::vector<Description *> rightOrder(systems.size());

    for (uint i = 0; i < systems.size(); i++)
      rightOrder[answer[i]] = systems[i];

    swap(systems, rightOrder);
  }

  void SceneManager::start()
  {
    create_all_resources_from_metadata();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    topological_sort(get_all_systems());
    for (auto &[srcHandlers, filteredHandlers] : get_all_event_handlers())
    {
      topological_sort(filteredHandlers);
    }

  }
  void SceneManager::sort_systems()
  {
    auto &systems = ecs::get_all_systems();
    std::sort(systems.begin(), systems.end(), system_comparator);
    act.begin = systems.begin();
    act.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->stage >= stage::before_render;});
    before_render.begin = act.end;
    before_render.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->stage >= stage::render;});
    render.begin = act.end;
    render.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->stage >= stage::ui;});
    ui.begin = render.end;
    ui.end = std::find_if(systems.begin(), systems.end(),
      [](const SystemDescription *a){return a->stage >= stage::ui_menu;});
    menu.begin = ui.end;
    menu.end = systems.end();
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
    load_scene(currentScene.rawScene);
    send_event(OnSceneCreated());
  }
  void SceneManager::swap_editor_game_scene()
  {
    invalidate_cached_archetype();
    currentScene.inEditor = !currentScene.inEditor;
    init_scene();
  }
  void SceneManager::start_scene(const string &path, bool inEditor)
  {
    currentScene.rawScene = DataBlock(path);
    currentScene.path = path;
    currentScene.inEditor = inEditor;
    init_scene();
  }
  void SceneManager::init_scene()
  {
    bool inEditor = currentScene.inEditor;
    ecs::EntityContainer &curContainer = inEditor ? currentScene.editorScene : currentScene.gameScene;
    core().currentSceneTags = inEditor ? "editor" : "game";
    core().entityContainer = &curContainer;
    core().register_allowed_callable();
    sort_systems();
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
    {
      ProfilerLabel label((*it)->name.c_str());
      (*it)->execute();
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
      start_scene(sceneToLoad, currentScene.inEditor);
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