#include <filesystem>
#include <ostream>
#include <fstream>
#include "profile_tracker.h"

void ProfileTracker::stop()
{
  stopped = true;
  if (filesystem::exists(path))
  {
    for (int i = 1;true;i++)
    {
      string next_path = path.substr(0, path.size() - 4) + "(" + to_string(i) + ").csv";
      if (!filesystem::exists(next_path))
      {
        path = next_path;
        break;
      }
    }
  }
  filesystem::path p = path;
  p = p.parent_path();
  if(!filesystem::exists(p))
  {
    filesystem::create_directories(p);
  }
  ofstream os(path);

  os << "frame;time;average\n";
  double sum = 0;
  for (size_t i = 0; i < times.size(); i++)
  {
    sum += times[i];
    os << i+1 << ";" << times[i] << ";" << sum / (i+1) << endl;
  }
  os.close();
  debug_log("ProfileTrack wrote to %s", path.c_str());
}