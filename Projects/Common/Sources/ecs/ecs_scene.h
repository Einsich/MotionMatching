#pragma once
#include <vector>
#include <filesystem>
#include "manager/system_description.h"
#include "Engine/input.h"
#include "manager/entity_container.h"

namespace ecs
{
  struct Scene
  {
    string name;
    filesystem::path path;
    EntityContainer editorScene, gameScene;
    bool inEditor;
    bool gamePaused;
  };
  class SceneManager
  {
  private:
    typedef std::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange logic, render, ui, menu;
    uint currentSceneTags;
    vector<Scene*> scenes;
    Scene *currentScene;
    void update_range(const SystemRange &range);
    void destroy_entities(bool without_copy);
    void process_only_events();
  public:
    void save_current_scene();
    void start(bool editor);
    bool try_start_scene(const string &name, uint tags);
    void swap_editor_game_scene(bool pause);
    void update_logic();
    void update_render();
    void update_ui();
    void process_events();
    void destroy_scene();
    void add_open_scene(const filesystem::path &path, bool need_to_add, bool need_to_open);
  };
}