#include "ecs_scene.h"
#include <ecs/ecs.h>
#include "glad/glad.h"
#include <profiler/profiler.h>
#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <ecs/imgui.h>

#include <application/application_data.h>
#include <ecs/event_registration.h>

ECS_EVENT_REGISTRATION(ImguiRender)
ECS_EVENT_REGISTRATION(ImguiMenuRender)


void load_scene(const DataBlock &scene);

void SceneManager::pre_act()
{
  PROFILER(ecs_events);
  ecs::update_query_manager();

  perfLabel = ProfilerLabel("act");
}

void SceneManager::pre_before_render()
{

  perfLabel = ProfilerLabel("before_render");
  perfLabelGPU = ProfilerLabelGPU("frame");

  Application::instance().get_context().swap_buffer();

  extern void process_gpu_time_queries();
  process_gpu_time_queries();
  get_gpu_profiler().start_frame();

}
void SceneManager::pre_render()
{
  perfLabel = ProfilerLabel("ecs_render");
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void SceneManager::update_ui()
{
  perfLabel.stop();

  Application::instance().get_context().start_imgui();

  {
    PROFILER(ecs_imgui);
    ecs::send_event_immediate(ImguiRender());
    if (ImGui::BeginMainMenuBar())
    {
      ecs::send_event_immediate(ImguiMenuRender());
      ImGui::EndMainMenuBar();
    }
  }
  
  ProfilerLabelGPU imgui_render_label("imgui_render");
  PROFILER(imgui_render);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


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