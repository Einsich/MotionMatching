#include <ecs/ecs.h>
#include <profiler/profiler.h>

#include <3dmath.h>
#include <eastl/stack.h>
#include <memory/tmp_allocator.h>
#include <imgui.h>
#include <ecs/imgui.h>

void profiler(Profiler &profiler)
{
  auto& history = profiler.get_frame_history();
  if (history.size() == 0)
  {
    ImGui::End();
    return;
  }
  //ImVec2 corner = ImGui::GetWindowPos();
  //float width = ImGui::GetWindowWidth();
  //float height = 10.f;
  float maxdt = profiler.get_averange(history.back());

  eastl::stack<float, eastl::vector<float, tmp_allocator>> openTimes;
  openTimes.push(0.f);

  //ImDrawList* draw_list = ImGui::GetWindowDrawList();
  float lastCloseTime = 0.f;
  for (const TimeLabel &label : history)
  {
    float dt = profiler.get_averange(label);
    float spike = profiler.get_max(label);
    int level = openTimes.size();
    if (label.open)
    {
      openTimes.push(lastCloseTime);
      float hardness = sqrt(dt / maxdt);
      vec3 color = glm::lerp(vec3(1), vec3(1,0,0), hardness);
      ImGui::TextColored(ImVec4(color.x, color.y, color.z, 1.f), "%*c%s: avg %.2f, max %.2f ms",level*3, ' ', label.label, dt, spike);

    }
    else
    {
      if (openTimes.empty())
        continue;
      float dt = profiler.get_averange(label);
      float openTime = openTimes.top();
      float closeTime = openTime + dt;
      lastCloseTime = closeTime;
      openTimes.pop();
    }
  }
}
EVENT() menu_profiler(const ImguiMenuRender&)
{

  if (ImGui::BeginMenu("Profiler"))
  {
    if (ImGui::BeginTabBar(""))
    {
      static int selectedProfiler = 0;//cpu
      if (ImGui::TabItemButton("cpu"))
        selectedProfiler = 0;
      else if (ImGui::TabItemButton("gpu"))
        selectedProfiler = 1;
      
      profiler(selectedProfiler == 0 ? get_cpu_profiler() : get_gpu_profiler());
      
      ImGui::EndTabBar();
    }
    ImGui::EndMenu();
  }
}


extern size_t x;
size_t x = (uintptr_t)&x;

#include <cstdlib>

struct S
{
  S()
  {
    std::abort();
  }
};
static S s;

