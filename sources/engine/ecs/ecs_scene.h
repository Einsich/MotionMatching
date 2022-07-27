#pragma once
#include <vector>
#include <filesystem>
#include "manager/system_description.h"
#include "input.h"
#include "manager/entity_manager.h"
#include <data_block/data_block.h>

namespace ecs
{
  class SceneManager
  {
  private:
    SystemStageInterval act, before_render, render, render_ui, ui, menu;

    DataBlock rawScene;
    std::string scenePath;
    std::unique_ptr<EntityManager> editorScene, gameScene;

    bool inEditor;
    void sort_systems();
  public:
    std::string sceneToLoad;
    void restart_cur_scene();
    void start();
    void start_scene(const std::string &path, bool editor);
    void swap_editor_game_scene();
    void update_act();
    void update_render();
    void update_ui();
    void process_events();
    void destroy_scene();
  };
}