#include "profiler.h"
#include <ecs/hash.h>


ProfilerLabel::ProfilerLabel(const char *label):
 start(std::chrono::high_resolution_clock::now().time_since_epoch().count()), label(label), stopped(false)
{
  get_cpu_profiler().open_label(label);
}
void ProfilerLabel::stop()
{
  float dt = (std::chrono::high_resolution_clock::now().time_since_epoch().count() - start) * 0.000001;
  get_cpu_profiler().close_label(dt, label);
  stopped = true;
}
ProfilerLabel::~ProfilerLabel()
{
  if (!stopped)
    stop();
}
static uint label_id_hash(const char *label, int id)
{
  return ecs::hash(label) ^ (id * 16397816463u);
}

void Profiler::start_frame()
{
  prev_frame_labels = std::move(cur_frame_labels);
  cur_frame_labels.clear();
  labelCount.clear();
}

void Profiler::open_label(const char *label)
{
  int id = ++labelCount[ecs::hash(label)];
  cur_frame_labels.push_back({id, label, true});
}
void Profiler::close_label(float time_ms, const char *label)
{
  //support only flat similar label names
  int id = labelCount[ecs::hash(label)];
  cur_frame_labels.push_back({id, label, false});
  labelAveranges[label_id_hash(label, id)].add_time(time_ms);
}
float Profiler::get_averange(const TimeLabel &label)
{
  return labelAveranges[label_id_hash(label.label, label.id)].get_averange();
}

float Profiler::get_max(const TimeLabel &label)
{
  return labelAveranges[label_id_hash(label.label, label.id)].get_max();
}

const vector<TimeLabel> &Profiler::get_frame_history()
{
  return prev_frame_labels;
}

Profiler &get_cpu_profiler()
{
  static Profiler profiler;
  return profiler;
}

constexpr int framesCount = 60;

Profiler::AverangeTime::AverangeTime():dtChain(framesCount, 0.0f), curIndex(0), curSum(0){}

float Profiler::AverangeTime::get_averange() const
{
  return curSum / framesCount;
}

float Profiler::AverangeTime::get_max() const
{
  float maxTime = 0;
  for (float dt : dtChain)
    maxTime = max(maxTime, dt);
  return maxTime;
}

void Profiler::AverangeTime::add_time(float dt)
{
  curSum += -dtChain[curIndex] + dt;
  dtChain[curIndex] = dt;
  curIndex = (curIndex + 1) % framesCount;
}
