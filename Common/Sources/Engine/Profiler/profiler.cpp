#include "profiler.h"

ProfilerLabel::ProfilerLabel(const char *label):
 start(SDL_GetTicks()), label(label), stopped(false)
{
}
void ProfilerLabel::stop()
{
  get_profiler().add_label(start, SDL_GetTicks(), label);
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
  cur_frame_labels.push_back({start, end, label});
}
const vector<TimeLabel> &Profiler::get_frame_history() const
{
  return prev_frame_labels;
}

Profiler &get_profiler()
{
  static Profiler profiler;
  return profiler;
}