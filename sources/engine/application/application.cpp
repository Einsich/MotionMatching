#include "application_data.h"
#include "application.h"
#include "render/shader/shader_factory.h"
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "profiler/profiler.h"
#include <SDL2/SDL.h>
#include "template/blk_template.h"
#include "system_tag.h"
#include "ecs_scene.h"
#include "application_metainfo.h"

namespace ecs
{
  void load_templates_from_blk();
}

Application::Application(const string &project_name,const string &root, int width, int height, bool full_screen):
context(project_name, width, height, full_screen), timer(), scene(new ecs::SceneManager()),
root(root),
projectPath(root + "/" + project_name)
{
  assert(scene);
  application = this;
}

static void copy_paths(const string &root, const vector<string> &src, vector<filesystem::path> &dst)
{
  dst.resize(src.size());
  for (int i = 0, n = src.size(); i < n; i++)
    dst[i] = filesystem::path(root + "/" + src[i]);
}

void Application::start()
{
  load_meta_info(projectPath + "/project.config");
  ApplicationMetaInfo &metaInfo = get_meta_info();
  copy_paths(root, metaInfo.shadersPaths, shadersPaths);
  copy_paths(root, metaInfo.resourcesPaths, resourcesPaths);
  compile_shaders();
  bool editor = !metaInfo.startGame;
  
  #ifdef RELEASE
  editor = false;
  #endif
  scene->start();
  ecs::load_templates_from_blk();
  get_profiler();
  string sceneName;
  uint tags = editor ? ecs::SystemTag::Editor : ecs::SystemTag::Game;
  scene->start_scene(root_path(metaInfo.firstScene), tags);
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
  while(running){
    get_profiler().start_frame();
    PROFILER(main_loop);
    timer.update();
    PROFILER(sdl_events) 
		running = sdl_event_handler();
    sdl_events.stop();
    if (running)
    {
      PROFILER(ecs_events);
      scene->process_events();
      ecs_events.stop();
      PROFILER(ecs_logic);
      scene->update_logic();
      ecs_logic.stop();
      
      PROFILER(swapchain);
      context.swap_buffer();
      swapchain.stop();
      PROFILER(ecs_render);
      scene->update_render();
      ecs_render.stop();
      
      PROFILER(ui)
      context.start_imgui();
      PROFILER(ecs_ui);
      scene->update_ui();
      ecs_ui.stop();
      PROFILER(imgui_render);
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      imgui_render.stop();
      ui.stop();
    }
    main_loop.stop();
    get_profiler().end_frame();
	}
}
void Application::exit()
{
  save_shader_info();
  scene->destroy_scene();
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
