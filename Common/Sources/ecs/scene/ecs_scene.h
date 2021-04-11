#pragma once
#include <vector>
#include "Scene/iscene.h"
#include "../manager/system_description.h"
namespace ecs
{
  class Scene : IScene
  {
  private:
    typedef std::vector<SystemDescription*>::iterator SystemIterator;
    struct SystemRange { SystemIterator begin, end; };
    SystemRange logic, render, ui;
    void update_range(const SystemRange &range);
  public:
    void start_scene() override;
    void update_logic() override;
    void update_render() override;
    void update_ui() override;
    void process_events() override;
    void destroy_scene() override;
  };
}