#include <ecs/ecs.h>
#include <Engine/Profiler/profiler.h>
#include <stack>
#include <3dmath.h>

void profiler()
{
  auto& history = get_profiler().get_frame_history();
  if (history.size() == 0)
  {
    ImGui::End();
    return;
  }
  //ImVec2 corner = ImGui::GetWindowPos();
  //float width = ImGui::GetWindowWidth();
  //float height = 10.f;
  float maxdt = get_profiler().get_averange(history.back().label);

  stack<float> openTimes;
  openTimes.push(0.f);

  //ImDrawList* draw_list = ImGui::GetWindowDrawList();
  float lastCloseTime = 0.f;
  for (const TimeLabel &label : history)
  {
    float dt = get_profiler().get_averange(label.label);
    int level = openTimes.size();
    if (label.open)
    {
      openTimes.push(lastCloseTime);
      float hardness = sqrt(dt / maxdt);
      vec3 color = glm::lerp(vec3(1), vec3(1,0,0), hardness);
      ImGui::TextColored(ImVec4(color.x, color.y, color.z, 1.f), "%*c%s: %.2f ms",level*3, ' ', label.label.c_str(), dt);

    }
    else
    {
      float dt = get_profiler().get_averange(label.label);
      float openTime = openTimes.top();
      float closeTime = openTime + dt;
      lastCloseTime = closeTime;
      openTimes.pop();
      /*

      float c = dt / maxdt;
      auto color = ImGui::ColorConvertFloat4ToU32(ImVec4(1, c, c,1.f));
      ImVec2 p_min = ImVec2((openTime) * scale + corner.x, 100+level * height + corner.y);
      ImVec2 p_max = ImVec2((closeTime) * scale + corner.x, 100+(level+1) * height + corner.y);
      draw_list->AddRectFilled(p_min, p_max, color);
      */
    }
  }
}
SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::GameEditor) menu_profiler()
{

  if (ImGui::BeginMenu("Profiler"))
  {
    profiler();
    ImGui::EndMenu();
  }
}