#pragma once
#include "common.h"
#include "Scene/iscene.h"
#include "context.h"
#include "Event/input.h"
#include "Time/time.h"

class Application
{
private:
  inline static Application *application;
  IScene *scene;
  Context context;
  Time timer;
public:
  const string projectPath, projectResourcesPath, projectShaderPath, commonResourcesPath, commonShaderPath;
  Application(IScene *scene, string window_name, int width, int height, bool full_screen = false);
  bool sdl_event_handler();
  void start();
  void main_loop();
  void exit();
  static Application& instance()
  {
    return *application;
  }
  static Context& get_context()
  {
    return application->context;
  }
  static const Time& get_timer()
  {
    return application->timer;
  }
};
string project_resources_path(const string &path);
string common_resources_path(const string &path);