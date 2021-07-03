#pragma once
#include "common.h"
#include "context.h"
#include "Engine/input.h"
#include "Engine/time.h"

namespace ecs
{
  class Scene;
}
class Application
{
private:
  inline static Application *application;
  Context context;
  Time timer;
public:
  ecs::Scene *scene;
  const string projectPath, projectResourcesPath, projectShaderPath, commonResourcesPath, commonShaderPath;
  Application(string window_name, int width, int height, bool full_screen = false);
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
void load_editor_scene(const string &name);
void load_game_scene(const string &name);

string project_resources_path(const string &path);
string common_resources_path(const string &path);