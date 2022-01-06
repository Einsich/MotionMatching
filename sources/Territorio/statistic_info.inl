#include <ecs.h>
#include <imgui.h>



template<typename Callable>
void gather_lands(Callable);

SYSTEM(ecs::SystemOrder::UI) show_statistic(vector<vec3> &land_colors)
{
  struct StatInfo
  {
    uint landIndex;
    int landCount;
    uint forces;
    bool operator<(const StatInfo &other)
    { 
      return landCount > other.landCount;
    }
  };
  vector<StatInfo> info;
  QUERY(ecs::Tag isPlayableLand)gather_lands([&](uint landIndex, int landCount, uint forces)
  {
    info.emplace_back(StatInfo{landIndex, landCount, forces});
  });
  if (ImGui::Begin("score bar"))
  {
    std::sort(info.begin(), info.end());
    for (uint i = 0; i < info.size(); i++)
    {
      vec3 c = land_colors[info[i].landIndex];
      ImGui::TextColored(ImVec4(c.x, c.y, c.z, 1), "%d) lands %d, forces %d", i, info[i].landCount, info[i].forces);
    }
    ImGui::End();
  }
}