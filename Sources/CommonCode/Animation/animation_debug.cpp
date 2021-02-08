#include "animation_debug.h"

AnimationDebugRender::AnimationDebugRender(AnimationDataBasePtr dataBase):
debugSphere(create_sphere(Transform(vec3(), vec3()), 10)), dataBase(dataBase)
{
  poseMatchingWeights.resize(dataBase->clips.size());
  for (uint i = 0; i < poseMatchingWeights.size(); i++)
    poseMatchingWeights[i].resize(dataBase->clips[i].duration, 0);
  debugSphere->get_material()->set_property(Property("Ambient", vec3(1,1,1)));
}
void AnimationDebugRender::analyze_pose_matching(AnimationPlayerPtr animPlayer)
{
  if (animPlayer)
  {
    AnimationIndex index = animPlayer->get_current_animation();
    targetFeatures.clear();
    this->animPlayer = animPlayer;
    if (index.dataBase == dataBase && index.animation1 >= 0)
    {
      transform = animPlayer->gameObject->get_transform();
      const vector<AnimationClip> &animations = index.dataBase->clips;
      const AnimationFeatures & feature = index.dataBase->clips[index.animation1].features[index.cadr1];
      targetFeatures.push_back(&feature);
      best = 0;
      for(uint i = 0; i < animations.size(); i++)
      {
        for (uint j = 0, n = animations[i].features.size(); j < n; j++)
        {
          float pose_match = poseMatchingWeights[i][j] = pose_matching_norma(animations[i].features[j], feature);
          float goal_match = goal_matching_norma(animations[i].features[j].path, animations[i].tags, animPlayer->inputGoal);
          if (pose_match > best)
          {
            best = pose_match;
            besti = i;
            bestj = j;
          }
        }
      }
    }
  }
}

void AnimationDebugRender::show_ui_pose_matching()
{
  ImGui::Begin("Pose matching");
  const AnimationFeaturesWeightsPtr weights = dataBase->featureWeights;
  const vector<AnimationClip> &animations = dataBase->clips;
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImVec2 stringsSize = ImVec2(270, animations.size() * ImGui::GetTextLineHeightWithSpacing());
  for(const AnimationClip &animation : animations)
  {
    ImGui::Text("%s", animation.name.c_str());
  }
  ImGui::SliderFloat("pose match scale", &weights->norma_function_weight, 0, 10.f);
  for (const auto &p : weights->featureMap)
    ImGui::SliderFloat(p.first.c_str(), &weights->weights[(int)p.second], 0, 10);
  
  ImVec2 stringsPos = ImGui::GetWindowPos();
  ImVec2 pos = ImVec2(stringsPos.x + stringsSize.x, stringsPos.y + 1.75f * ImGui::GetTextLineHeightWithSpacing());
  ImVec2 size = ImVec2(3, 10);
  for(uint i = 0; i < poseMatchingWeights.size(); i++)
  {
    for (uint j = 0, n = poseMatchingWeights[i].size(); j < n; j++)
    {
      ImVec2 p = ImVec2(pos.x  + j * size.x, pos.y + i * ImGui::GetTextLineHeightWithSpacing()); 
      float t = poseMatchingWeights[i][j];
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x  + bestj * size.x, pos.y + besti * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f))); 

  ImGui::End();
}

void AnimationDebugRender::render_pose_matching(const Camera& mainCam, const DirectionLight& light)
{
  debugSphere->get_shader().use();
  for (const AnimationFeatures *feature : targetFeatures)
    if (feature)
    {
      debugSphere->get_material()->set_property(Property("Ambient", vec3(0,0,1)));
      debugSphere->get_transform().set_scale(vec3(0.1f));
      for (vec3 v: feature->features)
      {
        debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
        debugSphere->render(mainCam, light, true);
      }
      debugSphere->get_transform().set_scale(vec3(0.02f));
      debugSphere->get_material()->set_property(Property("Ambient", vec3(0,1,0)));
      for (vec3 v: feature->path.path)
      {
        debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
        debugSphere->render(mainCam, light, true);
      }
      debugSphere->get_material()->set_property(Property("Ambient", vec3(1,0,0)));
      for (vec3 v: animPlayer->inputGoal.path.path)
      {
        debugSphere->get_transform().get_position() = transform.get_transform() * vec4(v, 1.f);
        debugSphere->render(mainCam, light, true);
      }
    }
  
}