#pragma once 
struct SDL_Window;
typedef void *SDL_GLContext;
struct Resolution
{
  int width, height;
};

class Context
{
private:
  SDL_Window * window;
  SDL_GLContext gl_context;
  Resolution resolution;
public:
  Context(std::string window_name, int width, int height, bool full_screen = false);
  void start_imgui();
  void swap_buffer();
  Resolution get_resolution() const;
  float get_aspect_ratio() const;
};