#pragma once
#include <vector>
#include "manager/system_description.h"
#include "Engine/input.h"
#include "manager/entity_container.h"
namespace ecs
{
  struct SceneEntities
  {
    std::string name;
    EntityContainer editorScene, gameScene;
    bool gamePaused;
  };
  class Scene
  {
  private:
    typedef std::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange logic, render, ui, menu;
    uint currentSceneTags;
    std::vector<SceneEntities*> scenes;
    SceneEntities *currentScene;
    void update_range(const SystemRange &range);
    void destroy_entities(bool without_copy);
    void process_only_events();
  public:
    void start_scene();
    bool try_start_scene(const std::string &name, uint tags);
    void swap_editor_game_scene(bool pause);
    void update_logic();
    void update_render();
    void update_ui();
    void process_events();
    void destroy_scene();
  };
}