#pragma once
#include <vector>
#include "Engine/iscene.h"
#include "manager/system_description.h"
#include "Engine/input.h"
namespace ecs
{
  class Scene : IScene
  {
  private:
    typedef std::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange logic, render, ui;
    uint currentSceneTags;
    void update_range(const SystemRange &range);

    void keyboard_event_handler(const KeyboardEvent &event);
    void mouse_click_event_handler(const MouseClickEvent &event);
    void mouse_move_event_handler(const MouseMoveEvent &event);
    void mouse_wheel_event_handler(const MouseWheelEvent &event);
    void destroy_entities(bool without_copy);
    void process_only_events();
  public:
    void start_scene(uint32_t tags) override;
    void update_logic() override;
    void update_render() override;
    void update_ui() override;
    void process_events() override;
    void destroy_scene() override;
  };
}