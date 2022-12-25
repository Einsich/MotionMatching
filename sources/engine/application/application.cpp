#include "application_data.h"
#include "application.h"
#include "render/shader/shader_factory.h"
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "profiler/profiler.h"
#include <SDL2/SDL.h>
#include "ecs/ecs_scene.h"
#include "application_metainfo.h"
#include "memory/tmp_allocator.h"
#include <ecs/ecs.h>

namespace ecs_ex
{
  void load_templates_from_blk();
}
void create_all_resources_from_metadata();
void save_all_resources_to_metadata();

Application::Application(const string &project_name,const string &root, int width, int height, bool full_screen):
context(project_name, width, height, full_screen), timer(), scene(new SceneManager()),
root(root),
projectPath(root + "/" + project_name)
{
  assert(scene);
  application = this;
  ecs::init(false);
  ecs::init_singletones();
  ecs::init_stages({
    {"act", [&]() { scene->pre_act(); }, nullptr},
    {"before_render", [&]() { scene->pre_before_render(); }, nullptr},
    {"render", [&]() { scene->pre_render(); }, nullptr},
  });
  
  ecs::vector<ecs::string> editorTags, gameTags;
#ifndef RELEASE
  editorTags.emplace_back("debug");
  gameTags.emplace_back("debug");
#endif
  editorTags.emplace_back("editor");
  gameTags.emplace_back("game");
    
  ecs::set_system_tags(editorTags);

  ecs::pull_registered_files();
}

static void copy_paths(const std::string &root, const ecs::vector<ecs::string> &src, vector<filesystem::path> &dst)
{
  dst.resize(src.size());
  for (int i = 0, n = src.size(); i < n; i++)
    dst[i] = filesystem::path(root + "/" + src[i].c_str());
}

void Application::start()
{
  ApplicationMetaInfo metaInfo(projectPath + "/project.config");
  copy_paths(root, metaInfo.shadersPaths, shadersPaths);
  copy_paths(root, metaInfo.resourcesPaths, resourcesPaths);
  copy_paths(root, metaInfo.templatePaths, templatePaths);
  compile_shaders();
  bool editor = !metaInfo.startGame;
  
  #ifdef RELEASE
  editor = false;
  #endif
  scene->start();
  create_all_resources_from_metadata();
  ecs_ex::load_templates_from_blk();
  get_cpu_profiler();
  get_gpu_profiler();

  scene->start_scene(root_path(metaInfo.firstScene.c_str()), editor);
}
bool Application::sdl_event_handler()
{
  SDL_Event event;
  bool running = true;
  Input &input = Input::input();
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch(event.type){
      case SDL_QUIT: running = false; break;
      
      case SDL_KEYDOWN: 
      
      if(event.key.keysym.sym == SDLK_F2)
        scene->restart_cur_scene();
      
      if(event.key.keysym.sym == SDLK_F3)
        scene->swap_editor_game_scene();
      
      case SDL_KEYUP: input.event_process(event.key, Time::time());

      if(event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
        

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: input.event_process(event.button, Time::time()); break;

      case SDL_MOUSEMOTION: input.event_process(event.motion, Time::time()); break;

      case SDL_MOUSEWHEEL: input.event_process(event.wheel, Time::time()); break;
      
      case SDL_WINDOWEVENT: break;
    }
  }
  return running;
}
void Application::main_loop()
{
  bool running = true;
  while (running)
  {
    clear_tmp_allocation();
    get_cpu_profiler().start_frame();
    PROFILER(main_loop);
    timer.update();
    uint mainThreadJobsCount = mainThreadJobs.size();
    if (mainThreadJobsCount > 0)
    {
      for (uint i = 0; i < mainThreadJobsCount; i++)
        mainThreadJobs[i]();
      mainThreadJobs.erase(mainThreadJobs.begin(), mainThreadJobs.begin() + mainThreadJobsCount);
    }
    {
      PROFILER(sdl_events) 
		  running = sdl_event_handler();
    }
    if (running)
    {
      ecs::update_archetype_manager();
      ecs::perform_systems();
      scene->update_ui();
    }
	}
}
void Application::exit()
{
  save_shader_info();
  scene->destroy_scene();
  save_all_resources_to_metadata();
  delete scene;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}
string project_path(const string &path)
{
  return Application::instance().projectPath + "/" + path;
}
string project_path()
{
  return Application::instance().projectPath;
}
string root_path(const std::string &path)
{
  return Application::instance().root + "/" + path;
}
string root_path()
{
  return Application::instance().root;
}

void add_main_thread_job(std::function<void()> &&job)
{
  Application::instance().mainThreadJobs.emplace_back(std::move(job));
}

std::pair<int,int> get_resolution()
{
  Resolution r = Application::get_context().get_resolution();
  return {r.width, r.height};
}

void create_scene(const char *path)
{
  Application::instance().scene->sceneToLoad = path;
}