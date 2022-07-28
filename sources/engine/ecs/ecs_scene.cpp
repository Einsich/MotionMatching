#include "ecs_scene.h"
#include "ecs.h"
#include "ecs_event_impl.h"
#include "manager/entity_manager.h"
#include "glad/glad.h"
#include "profiler/profiler.h"
#include "imgui.h"

namespace ecs
{
  void load_scene(const DataBlock &scene);


  void SceneManager::start()
  {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    ecs::vector<ecs::string> editorTags, gameTags;
#ifndef RELEASE
    editorTags.emplace_back("debug");
    gameTags.emplace_back("debug");
#endif
    editorTags.emplace_back("editor");
    gameTags.emplace_back("game");
    editorScene = make_unique<ecs::EntityManager>(editorTags);
    gameScene = make_unique<ecs::EntityManager>(gameTags);
  }

  void SceneManager::sort_systems()
  {
    act = ecs::get_system_stage("act");
    before_render = ecs::get_system_stage("before_render");
    render = ecs::get_system_stage("render");
    render_ui = ecs::get_system_stage("render_ui");
    ui = ecs::get_system_stage("ui");
    menu = ecs::get_system_stage("ui_menu");
  }

  void SceneManager::swap_editor_game_scene()
  {
    inEditor = !inEditor;
    set_entity_manager(inEditor ? *editorScene : *gameScene);
    restart_cur_scene();
    sort_systems();
  }
  void SceneManager::start_scene(const std::string &path, bool in_editor)
  {
    if (path != scenePath)
    {
      rawScene = DataBlock(path);
      scenePath = path;
    }
    inEditor = in_editor;
    set_entity_manager(inEditor ? *editorScene : *gameScene);
    restart_cur_scene();
    sort_systems();
  }

  void SceneManager::restart_cur_scene()
  {
    entityManager->clear_scene();
    load_scene(rawScene);
    send_event(OnSceneCreated());

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
    update_range(render_ui);
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
    entityManager->process_events();

    if (sceneToLoad != "")
    {
      start_scene(sceneToLoad, inEditor);
      sceneToLoad = "";
    }
  }

  void SceneManager::destroy_scene()
  {
    ecs::send_event_immediate(OnSceneDestroy());
    entityManager->clear_scene();
  }
}