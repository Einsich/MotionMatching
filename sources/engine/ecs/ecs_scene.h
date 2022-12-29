#pragma once
#include <vector>
#include <filesystem>
#include <data_block/data_block.h>

class SceneManager
{
private:
  DataBlock rawScene;
  std::string scenePath;

  bool inEditor;
public:
  std::string sceneToLoad;
  void restart_cur_scene();
  void start();
  void start_scene(const std::string &path, bool editor);
  void swap_editor_game_scene();
  
  void destroy_scene();
};