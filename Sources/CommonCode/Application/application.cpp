#include "application.h"
#include "CommonCode/Shader/shader_factory.h"

Application::Application(string window_name, int width, int height):
scene(), context(window_name, width, height), timer(), input()
{
  application = this;
  compile_shaders(); 
  scene.init();
}
bool Application::sdl_event_handler()
{
  SDL_Event event;
  bool running = true;
  while (SDL_PollEvent(&event))
  {
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
      scene.update();
      scene.render();
      context.swap_buffer();
    }
	}
}
void Application::exit()
{
  SDL_Quit();
}