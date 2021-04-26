#pragma once

class IScene
{
public:
  virtual void start_scene() = 0;
  virtual void update_logic() = 0;
  virtual void update_render() = 0;
  virtual void update_ui() = 0;
  virtual void process_events() = 0;
  virtual void destroy_scene() = 0;
};