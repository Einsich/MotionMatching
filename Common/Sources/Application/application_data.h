#pragma once
#include <filesystem>
#include "common.h"
#include "context.h"
#include "Engine/input.h"
#include "Engine/time.h"

namespace ecs
{
  class SceneManager;
}
class Application
{
private:
  inline static Application *application;
  Context context;
  Time timer;
public:
  ecs::SceneManager *scene;
  filesystem::path projectPath, projectResourcesPath, projectShaderPath;
  Application(const string &window_name, const string &project_path, int width, int height, bool full_screen = false);
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