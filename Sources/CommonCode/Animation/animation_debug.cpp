#include "animation_debug.h"
AnimationDebugRender::AnimationDebugRender():
debugSphere(create_sphere(Transform(vec3(), vec3()), 10))
{
  debugSphere->get_material()->set_property(Property("Ambient", vec3(1,1,1)));
}


void AnimationDebugRender::show_ui_matching(AnimationPlayerPtr player)
{

  MotionMatchingBruteSolver* solver;
  AnimationDataBasePtr dataBase;
  if (!player || !(solver = dynamic_cast<MotionMatchingBruteSolver*>(player->get_motion_matching().get_solver().get())) 
      || !(dataBase = solver->get_data_base()))
    return;
  ImGui::Begin("Pose matching");
  const AnimationFeaturesWeightsPtr weights = dataBase->featureWeights;
  const vector<AnimationClip> &animations = dataBase->clips;
  const auto &matchingScore = solver->get_matching_scores();
  AnimationIndex cur = player->get_motion_matching().get_index().first;
  AnimationIndex next = player->get_motion_matching().get_index().second;
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImVec2 stringsSize = ImVec2(270, animations.size() * ImGui::GetTextLineHeightWithSpacing());
  for(const AnimationClip &animation : animations)
  {
    ImGui::Text("%s", animation.name.c_str());
  }
  ImGui::SliderFloat("pose match scale", &weights->norma_function_weight, 0, 5.f);
  ImGui::SliderFloat("goal scale", &weights->goal_weight, 0, 5.f);
  ImGui::SliderFloat("goal path weight", &weights->goal_path_weight, 0, 5.f);
  ImGui::SliderFloat("goal rotation", &weights->goal_rotation, 0, 15.f);
  ImGui::SliderFloat("goal tag weight", &weights->goal_tag_weight, 0, 25.f);
  ImGui::SliderFloat("next cadr weight", &weights->next_cadr_weight, 0, 5.f);
  for (const auto &p : weights->featureMap)
    ImGui::SliderFloat(p.first.c_str(), &weights->weights[(int)p.second], 0, 10);
  
  ImVec2 stringsPos = ImGui::GetWindowPos();
  ImVec2 pos = ImVec2(stringsPos.x + stringsSize.x, stringsPos.y + 1.75f * ImGui::GetTextLineHeightWithSpacing());
  ImVec2 size = ImVec2(3, 10);
  for(uint i = 0; i < matchingScore.size(); i++)
  {
    for (uint j = 0, n = matchingScore[i].size(); j < n; j++)
    {
      ImVec2 p = ImVec2(pos.x  + j * size.x, pos.y + i * ImGui::GetTextLineHeightWithSpacing()); 
      float t = matchingScore[i][j];
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x  + next.get_cadr_index() * size.x, pos.y + next.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 1.f ,1.f))); 
  p = ImVec2(pos.x + cur.get_cadr_index() * size.x, pos.y + cur.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f))); 
  ImGui::End();
}

void AnimationDebugRender::render_pose_matching(AnimationPlayerPtr player, const Camera& mainCam, const DirectionLight& light)
{
  if (!player)
    return;
  
  Transform transform = player->gameObject->get_transform();
  debugSphere->get_shader().use();

  const auto& feature = player->get_motion_matching().get_index().first.get_feature();

  debugSphere->get_material()->set_property(Property("Ambient", vec3(0,0,1)));
  debugSphere->get_transform().set_scale(vec3(0.1f));
  for (vec3 v: feature.features)
  {
    debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
    debugSphere->render(mainCam, light, true);
  }
  debugSphere->get_transform().set_scale(vec3(0.02f));
  debugSphere->get_material()->set_property(Property("Ambient", vec3(0,1,0)));
  for (vec3 v: feature.path.path)
  {
    debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
    debugSphere->render(mainCam, light, true);
  }
  debugSphere->get_material()->set_property(Property("Ambient", vec3(1,0,0)));
  for (vec3 v: player->inputGoal.path.path)
  {
    debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
    debugSphere->render(mainCam, light, true);
  }
    
  
}