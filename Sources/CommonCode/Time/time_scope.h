#pragma once
#include <SDL2/SDL_timer.h>
#include "CommonCode/common.h"
class TimeScope
{
private:
  Uint32 start;
  string message;
public:
  TimeScope(const string &message):
    start(SDL_GetTicks()), message(message)
  { }
  void stop()
  {
    Uint32 delta = SDL_GetTicks() - start;
    debug_log("%s\nspend %f seconds", message.c_str(), delta * 0.001f);
  }
  ~TimeScope()
  {
    stop();
  }
};