#pragma once
#include "common.h"
#include "Engine/iscene.h"
#include "context.h"
#include "Engine/input.h"
#include "Engine/time.h"

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
};
string project_resources_path(const string &path);
string common_resources_path(const string &path);