#include "profiler.h"

ProfilerLabel::ProfilerLabel(const char *label):
 start(std::chrono::high_resolution_clock::now().time_since_epoch().count()), label(label), stopped(false)
{
  get_profiler().open_label(start, label);
}
void ProfilerLabel::stop()
{
  get_profiler().close_label(start, std::chrono::high_resolution_clock::now().time_since_epoch().count(), label);
  stopped = true;
}
ProfilerLabel::~ProfilerLabel()
{
  if (!stopped)
    stop();
}

void Profiler::start_frame()
{
  prev_frame_labels = std::move(cur_frame_labels);
  cur_frame_labels.clear();
}
void Profiler::end_frame()
{

}
void Profiler::open_label(uint64_t start, const char *label)
{
  cur_frame_labels.push_back({start, label, true});
}
void Profiler::close_label(uint64_t start, uint64_t end, const char *label)
{
  cur_frame_labels.push_back({end, label, false});
  labelAveranges[label].add_time((end - start) * 0.000001);
}
float Profiler::get_averange(const char *label)
{
  return labelAveranges[label].get_averange();
}

float Profiler::get_max(const char *label)
{
  return labelAveranges[label].get_max();
}

const vector<TimeLabel> &Profiler::get_frame_history()
{
  return prev_frame_labels;
}

Profiler &get_profiler()
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
