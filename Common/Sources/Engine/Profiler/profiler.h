#pragma once
#include "Engine/time.h"
#include <vector>
class ProfilerLabel
{
private:
  Uint32 start;
  const string label;
  bool stopped;
public:
  ProfilerLabel(const char *label);
  ~ProfilerLabel();
  void stop();
};

struct TimeLabel {Uint32 start, end; string label;};
class Profiler
{
private:
  vector<TimeLabel> prev_frame_labels, cur_frame_labels;
public:
  void start_frame();
  void end_frame();
  void add_label(Uint32 start, Uint32 end, const string &label);
  const vector<TimeLabel> &get_frame_history() const;
};

Profiler &get_profiler();
#define PROFILER(label) ProfilerLabel label(#label);