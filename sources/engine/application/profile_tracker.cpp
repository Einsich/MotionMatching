#include <filesystem>
#include <ostream>
#include <fstream>
#include "profile_tracker.h"

void ProfileTracker::stop()
{
  stopped = true;
  string file_name = path.substr(0, path.size() - 4);//without .csv
  string histo_path = file_name + "_histogramm.csv";
  if (filesystem::exists(path))
  {
    for (int i = 1;true;i++)
    {
      string next_path = file_name + "(" + to_string(i) + ").csv";
      if (!filesystem::exists(next_path))
      {
        path = next_path;
        histo_path = file_name + "_histogramm" + "(" + to_string(i) + ").csv";
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
  constexpr float maxMsTime = 5.f;
  {
    ofstream os(path);

    os << "frame;time;average\n";
    double sum = 0;
    os << 0 << ";" << maxMsTime << ";" << 0.f << '\n';
    for (size_t i = 0; i < times.size(); i++)
    {
      float dt = times[i] * 1000.f;//in ms
      if (dt >= maxMsTime)
        dt = maxMsTime;
      times[i] = dt;
      sum += dt;
      os << i+1 << ";" << dt << ";" << sum / (i+1) << '\n';
    }
  }
  {
    ofstream os(histo_path);

    os << "time;count\n";
    constexpr int BinsCount = 500;
    vector<int> histogramm(BinsCount, 0);
    os << maxMsTime << ";" << 0 << '\n';
    for (size_t i = 0; i < times.size(); i++)
    {
      int binInd = (times[i] / maxMsTime) * BinsCount;
      binInd = clamp(binInd, 0, BinsCount - 1);
      histogramm[binInd]++;
    }

    for (size_t i = 0; i < BinsCount; i++)
    {
      if (histogramm[i] > 0)
      {
        float time = maxMsTime * i / BinsCount;
        os << time << ";" << histogramm[i] << '\n';
      }
    }
  }
  debug_log("ProfileTrack wrote to %s", path.c_str());
}