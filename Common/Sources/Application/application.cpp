#include "application.h"
#include "Engine/Render/Shader/shader_factory.h"
#include "glad/glad.h"
#include "Engine/imgui/imgui_impl_opengl3.h"
#include "Engine/imgui/imgui_impl_sdl.h"
#include "config.h"

Application::Application(IScene *scene,string window_name, int width, int height, bool full_screen):
scene(scene), context(window_name, width, height, full_screen), timer(),
projectPath(string(get_config("projectPath"))),
projectResourcesPath(string(get_config("projectPath")) + "/Resources"),
projectShaderPath(string(get_config("projectPath")) + "/Shaders"),
commonResourcesPath(string(get_config("commonPath")) + "/Resources"),
commonShaderPath(string(get_config("commonPath")) + "/Shaders")
{
  assert(scene);
  application = this;
  compile_shaders();
}
void Application::start()
{
  scene->start_scene();
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
      case SDL_KEYUP: input.event_process(event.key, Time::time());

      if(event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      break;

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
    timer.update();
    
		running = sdl_event_handler();
    if (running)
    {
      scene->process_events();
      scene->update_logic();
      scene->update_render();
      context.start_imgui();
      scene->update_ui();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      context.swap_buffer();
    }
	}
}
void Application::exit()
{
  scene->destroy_scene();
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