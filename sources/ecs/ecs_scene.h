#pragma once
#include <vector>
#include <filesystem>
#include "manager/system_description.h"
#include "input.h"
#include "manager/entity_container.h"
#include <data_block/data_block.h>

namespace ecs
{
  struct Scene
  {
    string path;
    DataBlock rawScene;
    EntityContainer editorScene, gameScene;
    bool inEditor;
  };
  class SceneManager
  {
  private:
    typedef ecs::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange act, before_render, render, ui, menu;
    Scene currentScene;
    void update_range(const SystemRange &range);
    void destroy_entities(bool without_copy);
    void sort_systems();
    void init_scene();
  public:
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