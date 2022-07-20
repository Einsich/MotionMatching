#include <stdio.h>
#include "glad/glad.h"
#include <SDL2/SDL.h>
#include <stdexcept>


struct SDL_Window;
typedef void *SDL_GLContext;


class Context
{
private:
  SDL_Window * window;
  SDL_GLContext gl_context;
public:
  Context(const char* window_name, int width, int height, bool full_screen = false)
  {
    SDL_Init(SDL_INIT_EVERYTHING);
    const char* glsl_version = "#version 450";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    size_t window_flags = SDL_WINDOW_OPENGL;
    if (full_screen)
      window_flags |= SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI ;
    window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, (SDL_WindowFlags)(window_flags));
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(0);
    
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
      throw std::runtime_error {"Glad error"};
    }

    glEnable              ( GL_DEBUG_OUTPUT );
  }

  void swap_buffer()
  {
    SDL_GL_SwapWindow(window);
  }
};

int main()
{
    printf("???\n");
  Context ctx("lolkek", 500, 600, true);
  while(1)
  {
    printf("hello\n");
    ctx.swap_buffer();
  }
}