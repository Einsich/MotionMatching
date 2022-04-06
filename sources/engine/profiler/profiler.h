#pragma once
#include "application/time.h"
#include <vector>
#include <map>
class ProfilerLabel
{
private:
  time_point start; 
  const char *label;
  bool stopped;
public:
  ProfilerLabel(const char *label);
  ~ProfilerLabel();
  void stop();
};

struct TimeLabel {time_point time; const char *label; bool open;};
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
    float get_max() const;
    void add_time(float dt);
  };
  struct cmp_str
  {
    bool operator()(char const *a, char const *b) const
    {
        return std::strcmp(a, b) < 0;
    }
  };
  map<const char*, AverangeTime, cmp_str> labelAveranges;
  vector<TimeLabel> prev_frame_labels, cur_frame_labels;
public:
  void start_frame();
  void end_frame();
  void open_label(time_point start, const char *label);
  void close_label(time_point start, time_point end, const char *label);
  float get_averange(const char *label);
  float get_max(const char *label);
  const vector<TimeLabel> &get_frame_history();
};

Profiler &get_profiler();
#define PROFILER(label) ProfilerLabel label(#label);