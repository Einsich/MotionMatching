#include "common.h"
#include "context.h"
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include <SDL2/SDL.h>
#include <profiler.h>


// During init, enable debug output
Context::Context(std::string window_name, int width, int height, bool full_screen)
{
  resolution = Resolution{width, height};
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
  window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.width, resolution.height, (SDL_WindowFlags)(window_flags));
  gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(0);
  
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
  {
    throw std::runtime_error {"Glad error"};
  }
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);
  SDL_GL_GetDrawableSize(window, &resolution.width, &resolution.height);
  glEnable              ( GL_DEBUG_OUTPUT );
}
void Context::start_imgui()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();
}
void Context::swap_buffer()
{
  PROFILER_EVENT("present")
  OPTICK_GPU_FLIP(window);
  SDL_GL_SwapWindow(window);
}
Resolution Context::get_resolution() const
{
  //SDL_GL_GetDrawableSize(window, &w, &h);
  return resolution;
}
float Context::get_aspect_ratio() const
{
  return (float)resolution.width / resolution.height;
}