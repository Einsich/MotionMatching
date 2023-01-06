#include "application_data.h"
#include "application.h"
#include "render/shader/shader_factory.h"
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <SDL2/SDL.h>
#include "ecs/ecs_scene.h"
#include "application_metainfo.h"
#include "memory/tmp_allocator.h"
#include <ecs/ecs.h>
#include "file_watcher.h"
#include <profiler.h>

namespace ecs_ex
{
  void load_templates_from_blk();
}
void create_all_resources_from_metadata();
void save_all_resources_to_metadata();

//daScript functionality
void init_dascript();
void close_dascript();

Application::Application(const std::string &project_name, const std::string &root, int width, int height, bool full_screen):
context(project_name, width, height, full_screen), timer(), scene(new SceneManager()),
root(root),
projectPath(root + "/" + project_name)
{
  assert(scene);
  application = this;
}

static void profiler_push(const char* label)
{
  OPTICK_PUSH_DYNAMIC(label);
}
static void profiler_pop()
{
  PROFILER_POP();
}

static void start_ecs()
{
  ecs::ecs_log = &debug_log;
  ecs::ecs_error = &debug_error;

  ecs::ecs_profiler_enabled = true;
  ecs::ecs_profiler_push = &profiler_push;
  ecs::ecs_profiler_pop = &profiler_pop;

  ecs::init(false);
  ecs::init_singletones();
  
  ecs::vector<ecs::string> editorTags, gameTags;
#ifndef RELEASE
  editorTags.emplace_back("debug");
  gameTags.emplace_back("debug");
#endif
  editorTags.emplace_back("editor");
  gameTags.emplace_back("game");
    
  ecs::set_system_tags(gameTags);

  ecs::pull_registered_files();
}

static void copy_paths(const std::string &root, const ecs::vector<ecs::string> &src, std::vector<filesystem::path> &dst)
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

  setup_directory_watch(metaInfo.scriptFolders);

  compile_shaders();
  bool editor = !metaInfo.startGame;
  
  #ifdef RELEASE
  editor = false;
  #endif
  scene->start();
  create_all_resources_from_metadata();
  ecs_ex::load_templates_from_blk();

  start_ecs();

  init_dascript();//should be called after start_ecs
  
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
    OPTICK_FRAME("MainThread");
    clear_tmp_allocation();
    timer.update();
    
    {
      PROFILER_EVENT("file watching");
      update_watching();
    }
    uint mainThreadJobsCount = mainThreadJobs.size();
    if (mainThreadJobsCount > 0)
    {
      for (uint i = 0; i < mainThreadJobsCount; i++)
        mainThreadJobs[i]();
      mainThreadJobs.erase(mainThreadJobs.begin(), mainThreadJobs.begin() + mainThreadJobsCount);
    }
    {
      PROFILER_EVENT("sdl_events");
		  running = sdl_event_handler();
    }
    if (running)
    {
      PROFILER_EVENT("ecs");
      ecs::update_archetype_manager();
      ecs::perform_deffered_events();

      ecs::perform_stage("act");
      Application::instance().get_context().swap_buffer();
      ecs::perform_stage("before_render");
      {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ecs::perform_stage("render");
      }
      Application::instance().get_context().start_imgui();

      {
        ecs::perform_stage("imgui_render");
        if (ImGui::BeginMainMenuBar())
        {
          ecs::perform_stage("imgui_menu");
          ImGui::EndMainMenuBar();
        }
      }
      
      PROFILER_EVENT("imgui_render");
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
	}
}
void Application::exit()
{
  save_shader_info();
  scene->destroy_scene();
  close_dascript();//after ecs destroy_scene
  save_all_resources_to_metadata();
  delete scene;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
  OPTICK_SHUTDOWN();
}
std::string project_path(const std::string &path)
{
  return Application::instance().projectPath + "/" + path;
}
const std::string &project_path()
{
  return Application::instance().projectPath;
}
std::string root_path(const std::string &path)
{
  return Application::instance().root + "/" + path;
}
const std::string &root_path()
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