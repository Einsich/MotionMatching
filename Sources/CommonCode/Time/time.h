#pragma once
#include "CommonCode/common.h"
#include <SDL2/SDL_timer.h>
#include <assert.h>

struct Time
{
private:
  Uint32 milliseconds;
  Uint32 millisecondDelta;
  float seconds;
  float secondDelta;
public:
  inline static Time* timer;
  Time():milliseconds(SDL_GetTicks()), millisecondDelta(0), seconds(milliseconds * 0.001f), secondDelta(0)
  {
    assert(timer == nullptr);
    timer = this;
  }
  ~Time()
  {
    assert(timer != nullptr);
    timer = nullptr;
  }
  void update()
  {
    Uint32 millisecondsNew = SDL_GetTicks();
    millisecondDelta = millisecondsNew - milliseconds;
    milliseconds = millisecondsNew;
    seconds = milliseconds * 0.001f;
    secondDelta = millisecondDelta * 0.001f;
  }
  static float time()
  {
    return timer->seconds;
  }
  static float delta_time()
  {
    return timer->secondDelta;
  }
  static float fps()
  {
    int t = timer->millisecondDelta < 1 ? 1 : timer->millisecondDelta;
    return 1000.f / t;
  }
};
