#pragma once
#include <vector>
#include <filesystem>
#include <profiler/profiler.h>
#include <data_block/data_block.h>

class SceneManager
{
private:
  ProfilerLabel perfLabel;
  ProfilerLabelGPU perfLabelGPU;
  DataBlock rawScene;
  std::string scenePath;

  bool inEditor;
public:
  std::string sceneToLoad;
  void restart_cur_scene();
  void start();
  void start_scene(const std::string &path, bool editor);
  void swap_editor_game_scene();
  
  void pre_act();
  void pre_before_render();
  void pre_render();
  
  void update_ui();
  void destroy_scene();
};