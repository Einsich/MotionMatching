
#include "ecs/ecs.h"
#include "Engine/imgui/imgui.h"
#include "Animation/animation_player.h"
#include "Animation/settings.h"
#include "Engine/Profiler/profiler.h"
#include <stack>

void show_settings(SettingsSet *settings, const char *label)
{
  ImGui::Begin(label);

  for (VarBase *var : settings->vars)
  {
    switch (var->varType)
    {
    case VarType::FLOAT: {Var<float>* fvar = (Var<float>*)var; ImGui::SliderFloat(var->name.c_str(), &fvar->value, fvar->min_val, fvar->max_val); break;}
    case VarType::INT:   {Var<int>* ivar = (Var<int>*)var; ImGui::SliderInt(var->name.c_str(), &ivar->value, ivar->min_val, ivar->max_val); break;}
    case VarType::BOOL:  {Var<bool>* bvar = (Var<bool>*)var; ImGui::Checkbox(var->name.c_str(), &bvar->value); break;}
    case VarType::LABEL: {ImGui::Text("%s", var->name.c_str());}
    
    default:
      break;
    }
  }
  ImGui::End();

}
void show_scores(const AnimationDataBasePtr dataBase, const MotionMatchingBruteSolver* solver, const MotionMatching &mm)
{

  ImGui::Begin("Scores");
  const vector<AnimationClip> &animations = dataBase->clips;
  const auto &matchingScore = solver->get_matching_scores();
  AnimationIndex cur = mm.get_index().current_index();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

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
      ImVec2 p = ImVec2(pos.x  + j * size.x, pos.y + i * ImGui::GetTextLineHeightWithSpacing()); 
      float t = 1.f / matchingScore[i][j] * 1.0f;//ADD GOOG WEIGHT
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x + cur.get_cadr_index() * size.x, pos.y + cur.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f))); 
  ImGui::End();
}
string tags_to_text(const set<AnimationTag> &tags)
{
  string tagsString = "";
  bool first = true;
  for (AnimationTag tag : tags)
  {
    if (!first)
      tagsString += ", ";
    tagsString += get_tag_name(tag);
    first = false;
  }
  return tagsString;
}
void show_best_score(const MatchingScores &score, const MotionMatching &mm, const set<AnimationTag> &tags)
{
  ImGui::Begin("Best score");
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 corner = ImGui::GetWindowPos();
  constexpr int N = 5;
  float scores[N];
  const char*names[N];
  vec3 colors[N] = {vec3(0.5f,0,0.7f), vec3(0.1f, 0.1f, 0.8f), vec3(0.7f, 0.3f, 0.f), vec3(0.1f, 0.9f, 0.9f), vec3(0.2f, 0.99f, 0.2f)};
  #define ADD_SCORE(i, SCORE) scores[i] = score.SCORE / score.full_score; names[i] = #SCORE;
  ADD_SCORE(0, pose)
  ADD_SCORE(1, goal_path)
  ADD_SCORE(2, goal_rotation)
  ADD_SCORE(3, goal_tag)
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
  
  ImGui::Text("goal tags { %s }", tags_to_text(tags).c_str());
  ImGui::Text("clips tags { %s }", tags_to_text(mm.get_index().current_index().get_clip().tags).c_str());
  bool loopable = mm.get_index().current_index().get_clip().loopable;
  if (loopable)
    ImGui::Text("[loopable]");
  
  ImGui::End();
}

void show_briefing()
{
  ImGui::Begin("Briefing");
  ImGui::Text(
    "Move - WASD. Run - shift+WASD. Crouch - Z.\n"
    "");
  ImGui::End();
}
void profiler_info()
{
  ImGui::Begin("Profiler");
  auto& history = get_profiler().get_frame_history();
  if (history.size() == 0)
  {
    ImGui::End();
    return;
  }
  ImVec2 corner = ImGui::GetWindowPos();
  float width = ImGui::GetWindowWidth();
  float height = 10.f;
  float maxdt = get_profiler().get_averange(history.back().label);
  float scale = (width - 10) / maxdt;
  stack<float> openTimes;
  openTimes.push(0.f);

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  float lastCloseTime = 0.f;
  for (const TimeLabel &label : history)
  {
    float dt = get_profiler().get_averange(label.label);
    int level = openTimes.size();
    if (label.open)
    {
      openTimes.push(lastCloseTime);
      ImGui::Text("%*c%s: %.2f ms",level*3, ' ', label.label.c_str(), dt);
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
  
  ImGui::End();
}

SYSTEM(ecs::SystemOrder::UI) ui_unique_render()
{
  profiler_info();
  show_briefing();
  show_settings(Settings::instance, "Controller property");
  //show_settings(TestSettings::instance, "Test property");
  show_settings(MotionMatchingWeights::instance, "Motion matching weights");
}
SYSTEM(ecs::SystemOrder::UI) ui_render(
  const AnimationPlayer &animationPlayer)
{

  if (!Settings::MatchingStatistic)
    return;
  if (!animationPlayer.get_motion_matching())
  {
    //debug_error("Hasn't motion matching realisation");
    return;
  }
  const MotionMatchingBruteSolver *solver 
    = dynamic_cast<MotionMatchingBruteSolver*>(animationPlayer.get_motion_matching()->get_solver().get());
  if (!solver)
  {
    debug_error("Hasn't motion matching colver");
    return;
  }
  AnimationDataBasePtr dataBase = solver->get_data_base();
  if(!dataBase)
  {
    debug_error("Hasn't database");
    return;
  }
  const MotionMatching &mm = *animationPlayer.get_motion_matching();

  show_scores(dataBase, solver, mm);

  show_best_score(solver->bestScore, mm, animationPlayer.inputGoal.tags);

}
