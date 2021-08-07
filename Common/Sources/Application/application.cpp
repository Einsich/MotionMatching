#include "application.h"
#include "Engine/Render/Shader/shader_factory.h"
#include "glad/glad.h"
#include "Engine/imgui/imgui_impl_opengl3.h"
#include "Engine/imgui/imgui_impl_sdl.h"
#include "config.h"
#include "Engine/Profiler/profiler.h"
#include <SDL2/SDL.h>
#include "ecs/editor/template.h"
#include "ecs/system_tag.h"
#include "ecs/ecs_scene.h"
#include "application_metainfo.h"

Application::Application(string window_name, int width, int height, bool full_screen):
context(window_name, width, height, full_screen), timer(), scene(new ecs::SceneManager()),
projectPath(string(get_config("projectPath"))),
projectResourcesPath(string(get_config("projectPath")) + "\\Resources"),
projectShaderPath(string(get_config("projectPath")) + "\\Shaders"),
commonResourcesPath(string(get_config("commonPath")) + "\\Resources"),
commonShaderPath(string(get_config("commonPath")) + "\\Shaders")
{
  assert(scene);
  application = this;
  compile_shaders();
}
void Application::start()
{
  ecs::load_templates();
  load_meta_info(projectPath + "/project.config");
  bool editor = false;
  #ifndef RELEASE
  editor = true;
  #endif
  ApplicationMetaInfo &metaInfo = get_meta_info();
  scene->start(editor);
  get_profiler();
  string sceneName;
  uint tags = editor ? (uint)ecs::SystemTag::Editor : (uint)ecs::SystemTag::Game;

  if (!scene->try_start_scene(metaInfo.firstScene, tags))
  {
    scene->try_start_scene("default", tags);
  }
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
        scene->swap_editor_game_scene(true);
      else
      if(event.key.keysym.sym == SDLK_F3)
        scene->swap_editor_game_scene(false);
      
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
      PROFILER(swapchain);
      context.swap_buffer();
    }
    main_loop.stop();
    get_profiler().end_frame();
	}
}
void Application::exit()
{
  ecs::save_templates();
  save_meta_info(projectPath + "/project.config");
  scene->destroy_scene();
  delete scene;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}
string project_resources_path(const string &path)
{
  return Application::instance().projectResourcesPath + "/" + path;
}
string common_resources_path(const string &path)
{
  return Application::instance().commonResourcesPath + "/" + path;
}

void load_scene(const string &name, bool editor)
{
  ecs::SceneManager &scene = *Application::instance().scene;
  if (!scene.try_start_scene(name, editor ? (uint)ecs::SystemTag::Editor : (uint)ecs::SystemTag::Game))
    debug_error("Can't load scene %s", name.c_str());
}

void add_open_scene(const filesystem::path &path, bool need_to_add, bool need_to_open)
{
  if (need_to_open)
    get_meta_info().firstScene = path.stem().string();
  Application::instance().scene->add_open_scene(path, need_to_add, need_to_open);
}