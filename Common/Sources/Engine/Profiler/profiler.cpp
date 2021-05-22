#include "profiler.h"

ProfilerLabel::ProfilerLabel(const char *label):
 start(SDL_GetTicks()), label(label), stopped(false)
{
}
void ProfilerLabel::stop()
{
  get_profiler().add_label(start, SDL_GetTicks(), label);
  stopped = true;
}
ProfilerLabel::~ProfilerLabel()
{
  if (!stopped)
    stop();
}

void Profiler::start_frame()
{
  //std::swap(prev_frame_labels, cur_frame_labels);
  prev_frame_labels = std::move(cur_frame_labels);
  //prev_frame_labels.clear();
  cur_frame_labels.clear();
}
void Profiler::end_frame()
{

}
void Profiler::add_label(Uint32 start, Uint32 end, const string &label)
{
  cur_frame_labels.push_back({start, end, label, 0.f});
  labelAveranges[label].add_time(end - start);
}
const vector<TimeLabel> &Profiler::get_frame_history()
{
  for (TimeLabel &label : prev_frame_labels)
    label.averange = labelAveranges[label.label].get_averange();
  return prev_frame_labels;
}

Profiler &get_profiler()
{
  static Profiler profiler;
  return profiler;
}

constexpr int framesCount = 30;

Profiler::AverangeTime::AverangeTime():dtChain(framesCount, 0.0f), curIndex(0), curSum(0){}

float Profiler::AverangeTime::get_averange() const
{
  return curSum / framesCount;
}
void Profiler::AverangeTime::add_time(float dt)
{
  curSum += -dtChain[curIndex] + dt;
  dtChain[curIndex] = dt;
  curIndex = (curIndex + 1) % framesCount;
}
