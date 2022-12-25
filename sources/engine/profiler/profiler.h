#pragma once
#include "application/time.h"
#include <vector>
#include <map>
class ProfilerLabel
{
private:
  uint64_t start; 
  const char *label = "";
  bool stopped = true;
public:
  ProfilerLabel():stopped(true){}
  ProfilerLabel(const char *label);
  ~ProfilerLabel();
  void stop();
};
class ProfilerLabelGPU
{
private:
  bool stopped;
  uint startQuery, endQuery;
  const char *label;
public:
  ProfilerLabelGPU():stopped(true){}
  ProfilerLabelGPU(const char *label);
  ~ProfilerLabelGPU();
  void stop();
};

struct TimeLabel {int id; const char *label; bool open;};
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

  map<uint64_t, AverangeTime> labelAveranges;
  map<uint64_t, int> labelCount;
  vector<TimeLabel> prev_frame_labels, cur_frame_labels;
public:
  void start_frame();
  void open_label(const char *label);
  void close_label(float time_ms, const char *label);
  float get_averange(const TimeLabel &label);
  float get_max(const TimeLabel &label);
  const vector<TimeLabel> &get_frame_history();
};

Profiler &get_cpu_profiler();
Profiler &get_gpu_profiler();
#define PROFILER(label) ProfilerLabel label(#label);