#include "ecs_scene.h"
#include <ecs/ecs.h>
#include "glad/glad.h"
#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


#include <application/application_data.h>
#include <ecs/event_registration.h>
#include <profiler.h>



void load_scene(const DataBlock &scene);



void SceneManager::start()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}


void SceneManager::swap_editor_game_scene()
{
  inEditor = !inEditor;
  restart_cur_scene();
}
void SceneManager::start_scene(const std::string &path, bool in_editor)
{
  if (path != scenePath)
  {
    rawScene = DataBlock(path);
    scenePath = path;
  }
  inEditor = in_editor;
  restart_cur_scene();
}

void SceneManager::restart_cur_scene()
{
  ecs::destroy_all_entities();
  ecs::destroy_sinletons();
  load_scene(rawScene);
  send_event(ecs::OnSceneCreated());

}

void SceneManager::destroy_scene()
{
  ecs::send_event_immediate(ecs::OnSceneTerminated());
  ecs::destroy_all_entities();
  ecs::destroy_sinletons();
}