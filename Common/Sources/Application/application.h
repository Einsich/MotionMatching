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
  const string projectPath, projectResourcesPath, projectShaderPath;
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
void load_scene(const string &name, bool editor);

string project_resources_path(const string &path);
string common_resources_path(const string &path);

void add_open_scene(const filesystem::path &path, bool need_to_add = true, bool need_to_open = false);