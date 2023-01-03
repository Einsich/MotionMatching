#include <filesystem>
#include <ostream>
#include <fstream>
#include "profile_tracker.h"
namespace filesystem = std::filesystem;

void ProfileTracker::stop()
{
  if (stopped)
    return;
  stopped = true;
  std::string file_name = path.substr(0, path.size() - 4);//without .csv
  std::string histo_path = file_name + "_histogramm.csv";
  if (filesystem::exists(path))
  {
    for (int i = 1;true;i++)
    {
      std::string next_path = file_name + "(" + std::to_string(i) + ").csv";
      if (!filesystem::exists(next_path))
      {
        path = next_path;
        histo_path = file_name + "_histogramm" + "(" + std::to_string(i) + ").csv";
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
  float maxMsTime = times[0];
  constexpr int BinsCount = 75;
  for (size_t i = 0; i < times.size(); i++)
  {
    times[i] *= 1000.f;//in ns
    maxMsTime = std::max(maxMsTime, times[i]);
  }
  
  {
    std::ofstream os(histo_path);

    os << "time;count\n";
    std::vector<int> histogramm(BinsCount, 0);
    for (size_t i = 0; i < times.size(); i++)
    {
      int binInd = (times[i] / maxMsTime) * BinsCount;
      binInd = std::clamp(binInd, 0, BinsCount - 1);
      histogramm[binInd]++;
    }

    for (size_t i = 0; i < BinsCount; i++)
    {
      float time = maxMsTime * (i+1) / BinsCount;
      os << time << ";" << histogramm[i] << '\n';
    }
  }
  {
    std::ofstream os(path);
    os << "frame;time;average\n";
    double sum = 0;
    int maxSamples = std::min(BinsCount, (int)times.size());
    int binSize = times.size() / maxSamples;
    for (int i = 0; i < maxSamples; i++)
    {
      float t = 0;
      for (int j = 0; j < binSize; j++)
      {
        t += times[i * binSize + j];
      }
      times[i] = t / maxSamples;
    }
    for (int i = 0; i < maxSamples; i++)
    {
      float dt = times[i];
      sum += dt;
      os << 10000*(i+1)/maxSamples << ";" << dt << ";" << sum / (i+1) << '\n';
    }
    maxTime = maxMsTime;
    averageTime = sum / maxSamples;
  }
  debug_log("ProfileTrack wrote to %s", path.c_str());
}