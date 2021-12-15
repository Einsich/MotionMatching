#pragma once
#include <vector>
#include <filesystem>
#include "manager/system_description.h"
#include "input.h"
#include "manager/entity_container.h"

namespace ecs
{
  struct Scene
  {
    string path;
    EntityContainer editorScene, gameScene;
  };
  class SceneManager
  {
  private:
    typedef std::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange logic, render, ui, menu;
    Scene currentScene;
    void update_range(const SystemRange &range);
    void destroy_entities(bool without_copy);
  public:
    void restart_cur_scene();
    void start();
    void start_scene(const string &path, uint tags);
    void swap_editor_game_scene();
    void update_logic();
    void update_render();
    void update_ui();
    void process_events();
    void destroy_scene();
  };
}