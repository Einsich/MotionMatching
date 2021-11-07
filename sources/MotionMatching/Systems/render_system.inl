#include <ecs.h>
#include <algorithm>
#include "Animation/animation_player.h"
#include <transform.h>
#include <render/render.h>
#include <render/debug_arrow.h>
#include <render/global_uniform.h>
#include "Animation/settings.h"
  
SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) process_animation(
  const Asset<Mesh> &mesh,
  const AnimationPlayer &animationPlayer,
  Transform &transform,
  const Settings &settings)
{
  if (mesh)
  {
    vector<mat4> &curTransform = transform.get_bones();
    const AnimationTree &tree = animationPlayer.tree;
  
    curTransform.resize(mesh->bonesMap.size());
    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      int idx = animationPlayer.treeBoneToMesh[i];
      if (idx >= 0)
        curTransform[idx] = tree.get_bone_transform(i);
    }
  }
  if (settings.debugBones)
  {
    mat4 t = transform.get_transform();
    const AnimationTree &tree = animationPlayer.tree;
    static vector<vec3> boneOffsets;
    if (boneOffsets.size() != tree.nodes.size())
      boneOffsets.resize(tree.nodes.size());

    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      boneOffsets[i] = vec3(tree.get_transform(i)[3]);
    }
    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      vec3 p = tree.nodes[i].parent() >= 0 ? boneOffsets[tree.nodes[i].parent()] : vec3(0.f);
      float width = std::min(0.5f, length(boneOffsets[i] - p) * 0.05f);
      draw_arrow(t, p, boneOffsets[i], vec3(0,0.8f,0), width);
    }
  }

  AnimationLerpedIndex index = animationPlayer.get_motion_matching() ? animationPlayer.get_motion_matching()->get_index() : animationPlayer.get_index();

  mat4 transformation = transform.get_transform();

  const AnimationFeatures& feature = index.current_index().get_feature();
  
  //vec3 man = transformation * vec4(feature.nodes[(int)AnimationFeaturesNode::Hips], 1.f);
  //Ray ray(man, vec3(0,-1,0), 100);
  //Collision collision = ray_cast(ray);
  //draw_arrow(ray.from, collision.collisionPoint, vec3(10,0,0), 0.04f, false);

  if (settings.debugNodes)
  {
    #define DEBUG_NODE(node)\
    {\
      vec3 p = vec3(transformation * vec4(feature.nodes[(int)node], 1.f));\
      vec3 v = vec3(transformation * vec4(feature.nodesVelocity[(int)node], 0.f));\
      draw_arrow(p, p + v * 0.5f, vec3(1,0,0), 0.02f, false);\
    }

    DEBUG_NODE(AnimationFeaturesNode::LeftToeBase)
    DEBUG_NODE(AnimationFeaturesNode::LeftHand)
    DEBUG_NODE(AnimationFeaturesNode::RightToeBase)
    DEBUG_NODE(AnimationFeaturesNode::RightHand)
    DEBUG_NODE(AnimationFeaturesNode::Hips)
  }
/* 
  if (false)
  {
    u8 onGround = index.current_index().get_clip().onGround[index.current_index().get_cadr_index()];
    onGround = animationPlayer.onGround;
    if (onGround & 1)
    {
      material->set_property(Property("Ambient", vec3(1,0,0)));
      debugTransform.set_scale(vec3(0.11f));
      
      debugTransform.get_position() = transformation * vec4(feature.nodes[(int)AnimationFeaturesNode::LeftToeBase], 1.f);
      debugGoalSphere.render(debugTransform, true);
    }
    if (onGround & 2)
    {
      material->set_property(Property("Ambient", vec3(1,0,0)));
      debugTransform.set_scale(vec3(0.11f));
      
      debugTransform.get_position() = transformation * vec4(feature.nodes[(int)AnimationFeaturesNode::RightToeBase], 1.f);
      debugGoalSphere.render(debugTransform, true);
    }
  } */
  if (settings.debugTrajectory)
  {
    constexpr float dirLength = 0.3f;
    constexpr vec3 colors[2] = {vec3(0,1,0), vec3(1,0,0)};
    constexpr float lenghts[2] = {0.3f, 0.3f};
    const AnimationTrajectory &trajectory = index.current_index().get_trajectory();

    const std::array<TrajectoryPoint,AnimationTrajectory::PathLength> *trajectories[2] = {&trajectory.trajectory, &animationPlayer.inputGoal.path.trajectory};
    for(int i = 0; i < 2; i++)
    {
      for (const TrajectoryPoint &p: *trajectories[i])
      {
        vec3 v = vec3(transformation * vec4(p.point, 1.f));
        vec3 w = vec3(transformation * vec4(quat(vec3(0, p.rotation, 0)) * vec3(0, 0, dirLength * lenghts[i]), 0.f));
        draw_arrow(v, v + w, colors[i], 0.02f, false);
      }
    }
  }
}


/* EVEN() debug_goal_copy_mat(const ecs::OnEntityCreated &, Asset<Mesh> &debugGoalSphere)
{
  debugGoalSphere.get_material() = debugGoalSphere.get_material().copy();
} */