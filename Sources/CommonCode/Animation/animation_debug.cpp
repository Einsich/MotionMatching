#include "animation_debug.h"
#include "imgui/imgui.h"
#include "CommonCode/Components/MeshRender/mesh_render.h"
#include "animation_player.h"
#include "CommonCode/Components/DebugTools/debug_arrow.h"
#include "CommonCode/Physics/physics.h"
#include "CommonCode/Application/config.h"

AnimationDebugRender::AnimationDebugRender():
debugSphere(make_game_object())
{
  debugSphere->add_component<Transform>(vec3(0,0,0));
  MaterialPtr material = 
  debugSphere->add_component<MeshRender>(create_sphere(10))->get_material();

  material->set_property(Property("Ambient", vec3(1,1,1)));
  material->set_property(Property("Diffuse", vec3(0,0,0)));
  material->set_property(Property("Specular", vec3(0,0,0)));
}


void AnimationDebugRender::ui_render()
{
  if (!get_bool_config("showMatchingStatistic"))
    return;
  AnimationPlayer * player = gameObject->get_component<AnimationPlayer>();
  MotionMatchingBruteSolver* solver;
  AnimationDataBasePtr dataBase;
  if (!player || !player->get_motion_matching() ||
  !(solver = dynamic_cast<MotionMatchingBruteSolver*>(player->get_motion_matching()->get_solver().get())) 
      || !(dataBase = solver->get_data_base()))
    return;
  ImGui::Begin("Pose matching");
  const AnimationFeaturesWeightsPtr weights = dataBase->featureWeights;
  const vector<AnimationClip> &animations = dataBase->clips;
  const auto &matchingScore = solver->get_matching_scores();

  MotionMatching &mm = *player->get_motion_matching();
  AnimationIndex cur = mm.get_index().first;
  AnimationIndex next = mm.get_index().second;
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  ImVec2 stringsSize = ImVec2(270, animations.size() * ImGui::GetTextLineHeightWithSpacing());
  for(const AnimationClip &animation : animations)
  {
    ImGui::Text("%s", animation.name.c_str());
  }
  ImGui::SliderFloat("scale", &weights->debug_scale, 0.f, 1.f);
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
      float t = matchingScore[i][j] * weights->debug_scale;
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x  + next.get_cadr_index() * size.x, pos.y + next.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 1.f ,1.f))); 
  p = ImVec2(pos.x + cur.get_cadr_index() * size.x, pos.y + cur.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f ,1.f))); 
  ImGui::End();
}

void AnimationDebugRender::render(const Camera& mainCam, const DirectionLight& light, bool wire_frame)
{
  if (!get_bool_config("showGoal"))
    return;
  AnimationPlayer * player = gameObject->get_component<AnimationPlayer>();
  if (!player)
    return;
  AnimationLerpedIndex index = player->get_motion_matching() ? player->get_motion_matching()->get_index() : player->get_index();

  
  Transform* transform = gameObject->get_component<Transform>();
  mat4 transformation = transform ? transform->get_transform() : mat4(1.f);

  MeshRender * meshRender = debugSphere->get_component<MeshRender>();
  transform = debugSphere->get_component<Transform>();
  if (!meshRender || ! transform)
    return;

  MaterialPtr material = meshRender->get_material();
  const auto& feature = index.first.get_feature();
  
  /*vec3 man = transformation * vec4(feature.features[(int)AnimationFeaturesNode::Hips], 1.f);
  Ray ray(man, vec3(0,-1,0), 100);
  Collision collision = ray_cast(ray);
  
  draw_arrow(ray.from, collision.collisionPoint, vec3(10,0,0), 0.04f, false);
*/
  u8 onGround = index.first.get_clip().onGround[index.first.get_cadr_index()];
  onGround = player->onGround;
  material->set_property(Property("Ambient", vec3(1,1,1)));
  transform->set_scale(vec3(0.1f));
  for (vec3 v: feature.features)
  {
    transform->get_position() = transformation * vec4(v, 1.f);
    meshRender->render(*transform, mainCam, light, true);
  }

  float r = feature.path.rotation;
  material->set_property(Property("Ambient", vec3(r,0,0)));
  transform->get_position() = transformation * vec4(0.1f,2,0, 1.f);
  meshRender->render(*transform, mainCam, light, true);

  material->set_property(Property("Ambient", vec3(-r,0,0)));
  transform->get_position() = transformation * vec4(-0.1f,2,0, 1.f);
  meshRender->render(*transform, mainCam, light, true);
  if (onGround & 1)
  {
    material->set_property(Property("Ambient", vec3(1,0,0)));
    transform->set_scale(vec3(0.11f));
    
    transform->get_position() = transformation * vec4(feature.features[(int)AnimationFeaturesNode::LeftToeBase], 1.f);
    meshRender->render(*transform, mainCam, light, true);
  }
  if (onGround & 2)
  {
    material->set_property(Property("Ambient", vec3(1,0,0)));
    transform->set_scale(vec3(0.11f));
    
    transform->get_position() = transformation * vec4(feature.features[(int)AnimationFeaturesNode::RightToeBase], 1.f);
    meshRender->render(*transform, mainCam, light, true);
  }

  transform->set_scale(vec3(0.02f));
  material->set_property(Property("Ambient", vec3(0,1,0)));
  for (vec3 v: feature.path.path)
  {
    transform->get_position() = transformation * vec4(v, 1.f);
    meshRender->render(*transform, mainCam, light, true);
  }
  material->set_property(Property("Ambient", vec3(1,0,0)));
  for (vec3 v: player->inputGoal.path.path)
  {
    transform->get_position() = transformation * vec4(v, 1.f);
    meshRender->render(*transform, mainCam, light, true);
  }
    
  
}