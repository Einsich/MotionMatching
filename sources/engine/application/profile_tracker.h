#pragma once
#include "time.h"

class ProfileTracker
{
private:
  string path;
  vector<float> times;
  size_t frameCount; 
  bool stopped;
public:
  ProfileTracker(const string &path, size_t frame_count):
    path(path), frameCount(frame_count), stopped(false)
  {
    times.reserve(frame_count);
  }
  void stop();

  void update(float dt)
  {
    if (!stopped)
    {
      times.emplace_back(dt);
      if (times.size() == frameCount)
        stop();
    }
  }
};

class ProfileTrack
{
private:
  time_point start;
public:
  ProfileTrack():
    start(std::chrono::high_resolution_clock::now())
  {

  }
  float delta() const
  {
    time_point end = std::chrono::high_resolution_clock::now();
    return duration_cast<std::chrono::duration<float>>(end - start).count();
  }
};
#define PROFILE_TRACK(path, frameCount)\
static ProfileTracker tracker(path, frameCount);\
ProfileTrack track(tracker);