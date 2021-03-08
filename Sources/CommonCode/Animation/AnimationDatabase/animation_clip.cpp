#include "animation_clip.h"

static map<string, vector<AnimationTag>> tagMap = {
{"MOB1_Crouch_To_Stand_Relaxed", {AnimationTag::Stay}},
{"MOB1_Stand_Rlx_Turn_In_Place_L_Loop", {AnimationTag::Stay}},
{"MOB1_Crouch_R_90", {AnimationTag::Crouch}},
{"MOB1_Walk_F_Loop", {AnimationTag::Stay, AnimationTag::Loopable}},
{"MOB1_Walk_F_Jump_RU", {AnimationTag::Jump}},
{"MOB1_Stand_Relaxed_R_90", {}},
{"MOB1_Jog_F_Loop", {AnimationTag::Stay, AnimationTag::Loopable}},
{"MOB1_Crouch_L_90", {AnimationTag::Crouch}},
{"MOB1_Crouch_Idle_V2", {AnimationTag::Crouch, AnimationTag::Loopable}},
{"MOB1_Stand_Relaxed_To_Jog_F", {AnimationTag::Stay}},
{"MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", {AnimationTag::Crouch}},
{"MOB1_Crouch_To_CrouchWalk_F", {AnimationTag::Crouch}},
{"MOB1_CrouchWalk_F_To_Crouch_RU", {AnimationTag::Crouch}},
{"MOB1_Stand_Relaxed_To_Crouch", {AnimationTag::Crouch}},
{"MOB1_Walk_F_Jump", {AnimationTag::Jump}},
{"MOB1_Stand_Rlx_Turn_In_Place_R_Loop", {AnimationTag::Stay}},
{"MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", {AnimationTag::Crouch}},
{"MOB1_Stand_Relaxed_Idle_v2", {AnimationTag::Stay, AnimationTag::Loopable}},
{"MOB1_Walk_F_To_Stand_Relaxed_RU", {AnimationTag::Stay}},
{"MOB1_Jog_F_Jump_RU", {AnimationTag::Jump}},
{"MOB1_Jog_F_Jump", {AnimationTag::Jump}},
{"MOB1_Stand_Relaxed_L_90", {}},
{"MOB1_Stand_Relaxed_To_Walk_F", {AnimationTag::Stay}},
{"MOB1_CrouchWalk_F_Loop", {AnimationTag::Crouch}}
};


AnimationClip::AnimationClip(uint duration, float ticksPerSecond, const string &name,
 AnimationTreeData& tree, map<string, vector<quat>>& quats, map<string, vector<vec3>>& vecs):
 rootTranslationDelta(duration), rootRotationDelta(duration),
 duration(duration), ticksPerSecond(ticksPerSecond), name(name), tags(tagMap[name]), features(duration)
{
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    const string & nodeName = tree.nodes[i].name;
    auto itVec = vecs.find(nodeName);
    auto itQuat = quats.find(nodeName);
    vector<vec3> zeroVec = {vec3(0.f)};
    vector<quat> zeroQuat = {quat(1,0,0,0)};
    channels.emplace_back(duration, itVec == vecs.end() ? zeroVec : itVec->second, itQuat == quats.end() ? zeroQuat : itQuat->second);

    if (nodeName == "Hips")
      hipsChannelIndex = i;
  }
  vector<mat4> transfroms(tree.nodes.size());
  for (uint i = 0; i < duration; i++)
  {
    for (uint j = 0; j < tree.nodes.size(); j++)
    {
      const AnimationNodeData& node = tree.nodes[j];
      quat rotation = channels[j].get_rotation(i);
      vec nodeTranslation = node.translation;
      if (node.name == "Hips")
      {
        vec3 translation = channels[j].get_translation(i);
        rootTranslationDelta[i] = vec3(translation.x, 0, translation.z);
        channels[j].get_translation(i) = nodeTranslation = vec3(0, translation.y, 0);
      
        mat4 m = toMat4(rotation);
        float x, y, z;
        glm::extractEulerAngleXYZ(m, x, y, z);
        m = glm::eulerAngleXYZ(0.f, y, z);
        rootRotationDelta[i] = x;
        rotation = quat_cast(m);
      }
      rotation = node.rotation * rotation;
      if (channels[j].rotation_size() != 1 || i == 0)
        channels[j].get_rotation(i) = rotation;

      mat4 nodeTransform = tree.nodes[j].transform;
      mat4 rotationM = glm::toMat4(rotation);
      mat4 translation =  glm::translate(mat4(1.f), nodeTranslation);
      nodeTransform = translation * rotationM;
      if (tree.nodes[j].parent >= 0)
        nodeTransform = transfroms[tree.nodes[j].parent] * nodeTransform;
      transfroms[j] = nodeTransform;
      features[i].set_feature(tree.nodes[j].name, nodeTransform[3]);
    }    
  }
  ground_calculate();
  float pathLength = 0;
  for (uint i = 1; i < duration; i++)
    pathLength += length(rootTranslationDelta[i] - rootTranslationDelta[i-1]);
  if(pathLength < 0.3f)
  {
    for (uint i = 0; i < duration; i++)
    {
      channels[hipsChannelIndex].get_translation(i) += rootTranslationDelta[i];
      for (vec3 &f:features[i].features)
        f += rootTranslationDelta[i];
      rootTranslationDelta[i] = vec3(0.f);
    }
  }
  for (int i = duration - 1; i > 0; i--)
  {
    rootRotationDelta[i] -= rootRotationDelta[i - 1];
    rootTranslationDelta[i] -= rootTranslationDelta[i - 1];
  }
  rootRotationDelta[0] = 0;
  rootTranslationDelta[0] = vec3(0.f);

  for (uint i = 0; i < duration; i++)
  {
    vec3 dt = rootTranslationDelta[i];
    float dr = rootRotationDelta[i];
    AnimationPathFeature &pathFeature = features[i].path;
    pathFeature.rotation = 0;
    for (uint j = 0; j < AnimationPathFeature::PathLength; j++)
    {
      if (j != 0)
      {
        pathFeature.path[j] = pathFeature.path[j - 1];
      }
      for (uint k = 0; k < AnimationPathFeature::SkipCadres; k++)
      {
        uint nextInd = i + j * AnimationPathFeature::SkipCadres + k;
        if (nextInd < rootTranslationDelta.size())
        {
          dt = rootTranslationDelta[nextInd];
          dr = rootRotationDelta[nextInd];
        }
        pathFeature.path[j] += dt;
        pathFeature.rotation += dr;
      }
    }
  }
}
AnimationCadr AnimationClip::get_frame(uint i) const
{
  AnimationCadr frame;
  frame.nodeRotation.reserve(channels.size());
  for (uint j = 0; j < channels.size(); j++)
  {
    frame.nodeRotation.push_back(channels[j].get_rotation_c(i));
  }
  frame.nodeTranslation = channels[hipsChannelIndex].get_translation_c(i);
  frame.rootRotationDelta = rootRotationDelta[i];
  frame.rootTranslationDelta = rootTranslationDelta[i];
  return frame;
}
bool AnimationClip::contains_tag(AnimationTag tag) const
{
  return std::find(tags.begin(), tags.end(), tag) != tags.end();
}
void AnimationClip::leg_process(int leg_index, u8 leg)
{
  vector<float> h(features.size());
  vector<int> g(features.size());
  for (uint i = 0; i < h.size(); i++)
    h[i] = features[i].features[leg_index].y;

  constexpr float ground_value = 0.04f;
  g[0] = abs(h[0]) < ground_value ? 1 : 0;
  for (uint i = 1; i < h.size(); i++)
    if (abs(h[i]) < ground_value)
      g[i] = g[i - 1] + 1;
  
  for(int j = h.size() - 1; j >= 0; j--)
  {
    if (g[j] > 5)
    {
      while (j >= 0 && g[j] > 0)
      {
        onGround[j] |= leg;
        j--;
      }
    }
  }
    
}
void AnimationClip::ground_calculate()
{
  onGround.resize(features.size(), 0);
  leg_process((int)AnimationFeaturesNode::LeftToeBase, 1);
  leg_process((int)AnimationFeaturesNode::RightToeBase, 2);
}
size_t AnimationClip::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, duration);
  size += write(os, hipsChannelIndex);
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, channels);
  size += write(os, rootTranslationDelta);
  size += write(os, rootRotationDelta);
  size += write(os, features);
  return size;
}
size_t AnimationClip::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, duration);
  size += read(is, hipsChannelIndex);
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, channels);
  size += read(is, rootTranslationDelta);
  size += read(is, rootRotationDelta);
  size += read(is, features);
  tags = tagMap[name];
  ground_calculate();
  return size;
}
