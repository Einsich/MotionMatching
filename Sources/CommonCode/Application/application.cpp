#include "application.h"
#include "CommonCode/Shader/shader_factory.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

Application::Application(string window_name, int width, int height):
context(window_name, width, height), timer(), input()
{
  application = this;
  compile_shaders(); 
  scene = make_shared<Scene>();
  scene->init();
}
bool Application::sdl_event_handler()
{
  SDL_Event event;
  bool running = true;
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch(event.type){
      case SDL_QUIT: running = false; break;
      
      case SDL_KEYDOWN: 
      case SDL_KEYUP: input.event_process(event.key);

      if(event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
      break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: input.event_process(event.button); break;

      case SDL_MOUSEMOTION: input.event_process(event.motion); break;

      case SDL_MOUSEWHEEL: input.event_process(event.wheel); break;
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
    uint accumulated_time = Time::udelta_time();
    if(accumulated_time < 1)
    {
      SDL_Delay(1);
    }
    
		running = sdl_event_handler();
    if (running)
    {
      scene->update();
      scene->render();
      context.start_imgui();
      scene->render_ui();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      context.swap_buffer();
    }
	}
}
void Application::exit()
{
  scene->exit();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}