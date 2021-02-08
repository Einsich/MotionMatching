#include "animation_debug.h"

void show_statistics(AnimationFeaturesWeightsPtr weights, const vector<AnimationClip> &animations, const AnimationFeatures &feature)
{

  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImVec2 stringsSize = ImVec2(270, animations.size() * ImGui::GetTextLineHeightWithSpacing());
  for(const AnimationClip &animation : animations)
  {
    ImGui::Text("%s", animation.name.c_str());
  }
  ImGui::SliderFloat("pose match scale", &weights->norma_function_weight, 0, 1000);
  for (const auto &p : weights->featureMap)
    ImGui::SliderFloat(p.first.c_str(), &weights->weights[(int)p.second], 0, 10);
  
  ImVec2 stringsPos = ImGui::GetWindowPos();
  ImVec2 pos = ImVec2(stringsPos.x + stringsSize.x, stringsPos.y + 1.75f * ImGui::GetTextLineHeightWithSpacing());
  ImVec2 size = ImVec2(3, 10);
  int besti, bestj;
  float best = 0;
  for(uint i = 0; i < animations.size(); i++)
  {
    for (int j = 0, n = animations[i].features.size(); j < n; j++)
    {
      ImVec2 p = ImVec2(pos.x  + j * size.x, pos.y + i * ImGui::GetTextLineHeightWithSpacing()); 
      float pose_match = pose_matching_norma(animations[i].features[j], feature);
      float t = pose_match;
      if (t > best)
      {
        best = t;
        besti = i;
        bestj = j;
      }
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x  + bestj * size.x, pos.y + besti * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f)));
  
}

void debug_pose_matching(AnimationPlayerPtr animPlayer)
{
  ImGui::Begin("Pose matching");
  if (animPlayer)
  {
    AnimationIndex index = animPlayer->get_current_animation();
    if (index.dataBase && index.animation1 >= 0)
    show_statistics(index.dataBase->featureWeights, index.dataBase->clips, index.dataBase->clips[index.animation1].features[index.cadr1]);
  }
  ImGui::End();
}