#pragma once
#include "CommonCode/common.h"
#include "CommonCode/Scene/scene.h"
#include "context.h"
#include "CommonCode/Event/input.h"
#include "CommonCode/Time/time.h"

class Application
{
private:
  inline static Application *application;
  Scene scene;
  Context context;
  Time timer;
  Input input;
public:
  Application(string window_name, int width, int height, bool full_screen = false);
  bool sdl_event_handler();
  void main_loop();
  void exit();
  static Application& instance()
  {
    return *application;
  }
  static Scene& get_scene()
  {
    return application->scene;
  }
  static Context& get_context()
  {
    return application->context;
  }
  static const Time& get_timer()
  {
    return application->timer;
  }
  static Input& get_input()
  {
    return application->input;
  }
};
