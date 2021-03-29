#include "animation_debug.h"
#include "imgui/imgui.h"
#include "Components/MeshRender/mesh_render.h"
#include "animation_player.h"
#include "Components/DebugTools/debug_arrow.h"
#include "Physics/physics.h"
#include "Application/config.h"

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

void show_sliders(const AnimationFeaturesWeightsPtr weights)
{
  ImGui::Begin("Sliders");
  ImGui::SliderFloat("scale", &weights->debug_scale, 0.f, 100.f);
  ImGui::SliderFloat("lerp scale", &weights->animation_lerp, 0.f, 1.f);
  ImGui::SliderFloat("pose match scale", &weights->norma_function_weight, 0, 100.f);
  ImGui::SliderFloat("goal path weight", &weights->goal_path_weight, 0, 100.f);
  ImGui::SliderFloat("goal rotation", &weights->goal_rotation, 0, 105.f);
  ImGui::SliderFloat("goal tag weight", &weights->goal_tag_weight, 0, 25.f);
  ImGui::SliderFloat("next cadr weight", &weights->next_cadr_weight, 0, 10.f);
  ImGui::SliderFloat("noise_scale", &weights->noise_scale, 0, 10.f);
  for (const auto &p : weights->featureMap)
    ImGui::SliderFloat(p.first.c_str(), &weights->weights[(int)p.second], 0, 10);
  ImGui::End();
}
void show_scores(const AnimationDataBasePtr dataBase, const AnimationFeaturesWeightsPtr weights, const MotionMatchingBruteSolver* solver, const MotionMatching &mm)
{

  ImGui::Begin("Scores");
  const vector<AnimationClip> &animations = dataBase->clips;
  const auto &matchingScore = solver->get_matching_scores();
  AnimationIndex cur = mm.get_index().first;
  AnimationIndex next = mm.get_index().second;
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
      float t = 1.f / matchingScore[i][j] * weights->debug_scale;
      draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1.f - t,t, 0 ,1.f)));
    }
  }
  ImVec2 p = ImVec2(pos.x  + next.get_cadr_index() * size.x, pos.y + next.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
  draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + size.x, p.y + size.y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 1.f ,1.f))); 
  p = ImVec2(pos.x + cur.get_cadr_index() * size.x, pos.y + cur.get_clip_index() * ImGui::GetTextLineHeightWithSpacing()); 
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
  constexpr int N = 6;
  float scores[N];
  const char*names[N];
  vec3 colors[N] = {vec3(0.5f,0,0.7f), vec3(0.1f, 0.1f, 0.8f), vec3(0.7f, 0.3f, 0.f), vec3(0.1f, 0.9f, 0.9f), vec3(0.6f,0.6f, 0.1f), vec3(0.2f, 0.99f, 0.2f)};
  #define ADD_SCORE(i, SCORE) scores[i] = score.SCORE / score.full_score; names[i] = #SCORE;
  ADD_SCORE(0, pose)
  ADD_SCORE(1, goal_path)
  ADD_SCORE(2, goal_rotation)
  ADD_SCORE(3, goal_tag)
  ADD_SCORE(4, next_cadr)
  ADD_SCORE(5, noise)
  auto index = mm.get_index().first;
  ImGui::Text(" ");
  ImGui::Text(" full score %.2f, final norma %.2f, clip %s",score.full_score, score.final_norma, index.get_clip().name.c_str());
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
  
 // ImGui::Text(" goal tags { %s }", tags_to_text(tags).c_str());
  //ImGui::Text("clips tags { %s }", tags_to_text(mm.get_index().first.get_clip().tags).c_str());
  bool loopable = mm.get_index().first.get_clip().loopable;
  if (loopable)
    ImGui::Text("[loopable]");
  
  ImGui::End();
}
void AnimationDebugRender::ui_render()
{
  if (!get_bool_config("showMatchingStatistic"))
    return;
  AnimationPlayer * player = game_object()->get_component<AnimationPlayer>();
  MotionMatchingBruteSolver* solver;
  AnimationDataBasePtr dataBase;
  if (!player || !player->get_motion_matching() ||
  !(solver = dynamic_cast<MotionMatchingBruteSolver*>(player->get_motion_matching()->get_solver().get())) 
      || !(dataBase = solver->get_data_base()))
    return;
  const AnimationFeaturesWeightsPtr weights = dataBase->featureWeights;
  MotionMatching &mm = *player->get_motion_matching();
  show_sliders(weights);
  show_scores(dataBase, weights, solver, mm);

  show_best_score(solver->bestScore, mm, player->inputGoal.tags);
}

void AnimationDebugRender::render(const Camera& mainCam, const DirectionLight& light, bool)
{
  if (!get_bool_config("showGoal"))
    return;
  AnimationPlayer * player = game_object()->get_component<AnimationPlayer>();
  if (!player)
    return;
  AnimationLerpedIndex index = player->get_motion_matching() ? player->get_motion_matching()->get_index() : player->get_index();

  
  Transform* transform = game_object()->get_component<Transform>();
  mat4 transformation = transform ? transform->get_transform() : mat4(1.f);

  MeshRender * meshRender = debugSphere->get_component<MeshRender>();
  transform = debugSphere->get_component<Transform>();
  if (!meshRender || ! transform)
    return;

  MaterialPtr material = meshRender->get_material();
  const auto& feature = index.first.get_feature();
  AnimationTrajectory trajectory = index.first.get_trajectory();
  
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
    meshRender->render(transform, mainCam, light, true);
  }

  if (onGround & 1)
  {
    material->set_property(Property("Ambient", vec3(1,0,0)));
    transform->set_scale(vec3(0.11f));
    
    transform->get_position() = transformation * vec4(feature.features[(int)AnimationFeaturesNode::LeftToeBase], 1.f);
    meshRender->render(transform, mainCam, light, true);
  }
  if (onGround & 2)
  {
    material->set_property(Property("Ambient", vec3(1,0,0)));
    transform->set_scale(vec3(0.11f));
    
    transform->get_position() = transformation * vec4(feature.features[(int)AnimationFeaturesNode::RightToeBase], 1.f);
    meshRender->render(transform, mainCam, light, true);
  }

  transform->set_scale(vec3(0.02f));
  constexpr float dirLength = 0.3f;
  constexpr vec3 colors[2] = {vec3(0,1,0), vec3(1,0,0)};
  constexpr float lenghts[2] = {2, 2.5f};

  std::array<TrajectoryPoint,AnimationTrajectory::PathLength> *trajectories[2] = {&trajectory.trajectory, &player->inputGoal.path.trajectory};
  for(int i = 0; i < 2; i++)
  {
    material->set_property(Property("Ambient", colors[i]));
    for (TrajectoryPoint &p: *trajectories[i])
    {
      vec3 v = vec3(transformation * vec4(p.point, 1.f));
      vec3 w = vec3(transformation * vec4(quat(vec3(0, p.rotation, 0)) * vec3(0, 0, dirLength * lenghts[i]), 0.f));
      transform->get_position() = v;
      meshRender->render(transform, mainCam, light, true);
      draw_arrow(v, v + w, colors[i], 0.02f, false);
    }
  }
    
  
}