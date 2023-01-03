#pragma once
#include <filesystem>
#include <functional>
#include "common.h"
#include "context.h"
#include "input.h"
#include "application/time.h"
namespace filesystem = std::filesystem;


class SceneManager;

class Application
{
private:
  inline static Application *application;
  Context context;
  Time timer;
public:
  SceneManager *scene;
  std::string root, projectPath;
  std::vector<filesystem::path> resourcesPaths, shadersPaths, templatePaths;
  std::vector<std::function<void()>> mainThreadJobs;
  Application(const std::string &window_name, const std::string &project_path, int width, int height, bool full_screen = false);
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