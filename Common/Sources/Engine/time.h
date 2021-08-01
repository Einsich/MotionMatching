#pragma once
#include <SDL2/SDL_timer.h>
#include <assert.h>
#include "debug_log.h"
#include <string>
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
  static float udelta_time()
  {
    return timer->millisecondDelta;
  }
  static float fps()
  {
    const int N  = 60;
    static Uint32 s[N] = {};
    static Uint32 sum = 0;
    static int k = 0;
    int t = timer->millisecondDelta;

    sum -= s[k];
    s[k] = t;
    sum += t; 
    k = (k + 1) % N;
    return 1000.f  * N / sum;
  }
};
class TimeScope
{
private:
  Uint32 start;
  const std::string message;
  bool stopped;
public:
  TimeScope(const std::string &message):
    start(SDL_GetTicks()), message(message), stopped(false)
  { }
  void stop()
  {
    Uint32 delta = SDL_GetTicks() - start;
    debug_log("%s\nspend %f seconds", message.c_str(), delta * 0.001f);
    stopped = true;
  }
  ~TimeScope()
  {
    if (!stopped)
      stop();
  }
};
