#pragma once
#include "Engine/time.h"
#include "common.h"
#include <map>
class ProfilerLabel
{
private:
  Uint32 start; 
  const string label;
  bool stopped;
public:
  ProfilerLabel(const string &label);
  ~ProfilerLabel();
  void stop();
};

struct TimeLabel {Uint32 time; string label; bool open;};
class Profiler
{
private:
  struct AverangeTime 
  {
    vector<float> dtChain;
    int curIndex;
    float curSum;
    AverangeTime();
    float get_averange() const;
    void add_time(float dt);
  };

  map<string, AverangeTime> labelAveranges;
  vector<TimeLabel> prev_frame_labels, cur_frame_labels;
public:
  void start_frame();
  void end_frame();
  void open_label(Uint32 start, const string &label);
  void close_label(Uint32 start, Uint32 end, const string &label);
  float get_averange(const string &label);
  const vector<TimeLabel> &get_frame_history();
};

Profiler &get_profiler();
#define PROFILER(label) ProfilerLabel label(#label);