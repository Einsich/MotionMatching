
#include <ecs.h>
#include <imgui.h>
#include "Animation/animation_player.h"
#include "Animation/settings.h"
#include "Animation/third_person_controller.h"
#include <manager/type_info.h>

void show_scores(const AnimationDataBasePtr dataBase, const MotionMatching &mm)
{

  if (!ImGui::Begin("Scores"))
  {
    ImGui::End();
    return;
  }
  const vector<AnimationClip> &animations = dataBase->clips;
  const auto &matchingScore = dataBase->matchingScore;
  AnimationIndex cur = mm.get_index().current_index();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  static float scale = 10;
  ImGui::SliderFloat("Scale", &scale, 0, 100);
  ImVec2 stringsSize = ImVec2(270, animations.size() * ImGui::GetTextLineHeightWithSpacing());
  for(const AnimationClip &animation : animations)
  {
    ImGui::Text("%s", animation.name.c_str());
  }
  ImVec2 stringsPos = ImGui::GetWindowPos();
  stringsPos.y -= ImGui::GetScrollY();
  ImVec2 pos = ImVec2(stringsPos.x + stringsSize.x, stringsPos.y + 1.75f * ImGui::GetTextLineHeightWithSpacing());
  ImVec2 size = ImVec2(3, 10);
  for(uint i = 0; i < matchingScore.size(); i++)
  {
    for (uint j = 0, n = matchingScore[i].size(); j < n; j++)
    {
      ImVec2 p = ImVec2(pos.x  + j * size.x, pos.y + (i+1) * ImGui::GetTextLineHeightWithSpacing()); 
      float t = 1.f / matchingScore[i][j] * scale;
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x + cur.get_cadr_index() * size.x, pos.y + (cur.get_clip_index()+1) * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f))); 
  ImGui::End();
}


void show_best_score(const MatchingScores &score, const MotionMatching &mm, AnimationTags tags)
{
  if (!ImGui::Begin("Best score"))
  {
    ImGui::End();
    return;
  }
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 corner = ImGui::GetWindowPos();
  constexpr int N = 3;
  float scores[N];
  const char*names[N];
  vec3 colors[N] = {vec3(0.5f,0,0.7f), vec3(0.1f, 0.1f, 0.8f), vec3(0.7f, 0.3f, 0.f)};
  #define ADD_SCORE(i, SCORE) scores[i] = score.SCORE / score.full_score; names[i] = #SCORE;
  ADD_SCORE(0, pose)
  ADD_SCORE(1, goal_path)
  ADD_SCORE(2, goal_rotation)
  
  auto index = mm.get_index().current_index();

  ImGui::Text(" ");
  ImGui::Text(" full score %6.2f, clip %s",score.full_score,  index.get_clip().name.c_str());
  for (int i = 0; i < N; i++)
    ImGui::Text("   %s score %.2f", names[i], scores[i] * score.full_score);
  float sum = 0;
  ImVec2 barSize(300, 15);
  float stringH =  ImGui::GetTextLineHeightWithSpacing();
  ImVec2 barCorner(10, 30);
  ImVec2 linesCorner(10, barCorner.y + barSize.y + stringH);
  ImVec2 sqSize(stringH, stringH);

  for (int i = 0; i < N; i++)
  {
    ImVec2 p = ImVec2(corner.x + barCorner.x, corner.y + barCorner.y);
    p.x += sum * barSize.x;
    ImVec2 q = ImVec2(p.x + scores[i] * barSize.x, p.y + barSize.y);
    auto color = ImGui::ColorConvertFloat4ToU32(ImVec4(colors[i].x, colors[i].y, colors[i].z ,1.f));
    draw_list->AddRectFilled(p, q, color);
    sum += scores[i];
    p = ImVec2(corner.x + linesCorner.x+1, corner.y + linesCorner.y + sqSize.y * i+1);
    q = ImVec2(p.x + sqSize.x-2, p.y + sqSize.y-2);

    draw_list->AddRectFilled(p, q, color);
  }
  
  ImGui::Text("goal tags { %s }", tags.to_string().c_str());
  ImGui::Text("clips tags { %s }", mm.get_index().current_index().get_clip().tags.to_string().c_str());
  bool loopable = mm.get_index().current_index().get_clip().loopable;
  if (loopable)
    ImGui::Text("[loopable]");
  
  ImGui::End();
}

SYSTEM(ecs::SystemOrder::UI) briefing_ui()
{
  if (!ImGui::Begin("Briefing"))
  {
    ImGui::End();
    return;
  }
  ImGui::Text(
    "Move - WASD. Run - shift+WASD. Crouch - Z.\n"
    "Open Settings menu bar to enable camera rotation, inverse mouse or to turn on some debug information.\n"
    "");
  ImGui::End();
}

SYSTEM(ecs::SystemOrder::UI, ThirdPersonController thirdPersonController) motion_matching_statistic(
  const AnimationPlayer &animationPlayer,
  const Settings &settings)
{

  if (!settings.MatchingStatistic)
    return;
  if (!animationPlayer.get_motion_matching())
  {
    //debug_error("Hasn't motion matching realisation");
    return;
  }
  
  AnimationDataBasePtr dataBase = animationPlayer.dataBase;
  if(!dataBase)
  {
    debug_error("Hasn't database");
    return;
  }
  const MotionMatching &mm = *animationPlayer.get_motion_matching();

  show_scores(dataBase, mm);

  show_best_score(mm.bestScore, mm, animationPlayer.inputGoal.tags);

}
SYSTEM(ecs::SystemOrder::UI, ThirdPersonController thirdPersonController) current_anim_index(
  const AnimationPlayer &animationPlayer)
{
  ImGui::Begin("Current anim");
  AnimationIndex ind = animationPlayer.index.current_index();
  const AnimationClip &clip = ind.get_clip();
  
  ImGui::Text("played %s[%d], %d/%d", clip.name.c_str(), ind.get_clip_index(), ind.get_cadr_index(), clip.duration);
  ImGui::End();
}
template<typename T>
void display_property(T &property, const char *name)
{
  ImGui::Text("%s", name);
}
void display_property(float &property, const char *name)
{
  ImGui::InputFloat(name, &property, 0.1f, 10.f, 2);
}
void display_property(int &property, const char *name)
{
  ImGui::InputInt(name, &property);
}
void display_property(bool &property, const char *name)
{
  ImGui::Checkbox(name, &property);
}
void display_property(vec3 &property, const char *name)
{
  ImGui::InputFloat3(name, &property.x);
}
void display_property(vec4 &property, const char *name)
{
  ImGui::InputFloat4(name, &property.x);
}
template<typename T>
void show_settings(T &settings, const char *settings_name)
{
  if (ImGui::BeginMenu(settings_name))
  {
    const auto &f = [](auto &arg, const char *name){display_property(arg, name);};
    settings.reflect(f);
    ImGui::EndMenu();
  }
}
template<typename T>
void settings_manager(vector<pair<string, T>> &settings, const char *settings_name)
{
  constexpr int BUF_SIZE = 64;
  char buf[BUF_SIZE];
  static string lastName = "";

  if (ImGui::BeginMenu(settings_name))
  {
    static bool add = false, remove = false;

    if (add)
    {
      settings.emplace_back("preset" + to_string(settings.size()), settings.empty() ? T() : settings.back().second);
      settings.back().first.reserve(BUF_SIZE);
    }
    
    if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_AutoSelectNewTabs))
    {
      for (uint i = 0; i < settings.size(); ++i)
      {
        auto &p = settings[i];
        if (ImGui::BeginTabItem(p.first.c_str(), nullptr))
        {
          strcpy(buf, p.first.data());
          if (ImGui::InputText("preset name", buf, BUF_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
            p.first = std::string(buf);


          const auto &f = [](auto &arg, const char *name){display_property(arg, name);};
          p.second.reflect(f);

          ImGui::EndTabItem();
          if (remove)
          {
            settings.erase(settings.begin() + i);
            i--;
          }
        }

      }
      
      add = ImGui::TabItemButton("+");
    
      remove = ImGui::TabItemButton("-");
      ImGui::EndTabBar();
    }
    ImGui::EndMenu();
  }
}


SYSTEM(ecs::SystemOrder::UIMENU) menu_ui(
  Settings &settings,
  SettingsContainer &settingsContainer)
{
  show_settings(settings, "Settings");
  settings_manager(settingsContainer.controllerSettings, "Controller");
  settings_manager(settingsContainer.motionMatchingSettings, "Motion matching");
  settings_manager(settingsContainer.motionMatchingOptimisationSettings, "Motion matching optimisation");
}


SYSTEM(ecs::SystemOrder::UI) mm_early_text_perf(Settings &settings)
{

  ImGui::Begin("Early mm test");
  ImGui::Text("perf = %f %%", 100.f * (float)settings.earlyTestMMCount / settings.MMCount);
  ImGui::End();
}