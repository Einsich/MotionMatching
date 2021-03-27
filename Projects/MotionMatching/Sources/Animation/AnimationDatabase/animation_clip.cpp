#include "animation_clip.h"



AnimationClip::AnimationClip(uint duration, float ticksPerSecond, const string &name,
 AnimationTreeData& tree, map<string, vector<quat>>& quats, map<string, vector<vec3>>& vecs, const set<AnimationTag> &tags):
 hipsTranslation(duration), hipsRotation(duration),
 duration(duration), ticksPerSecond(ticksPerSecond), 
 loopable(std::find(tags.begin(), tags.end(), AnimationTag::Loopable) != tags.end()), 
 name(name), tags(tags), features(duration)
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
      vec3 nodeTranslation = node.translation;
      if (node.name == "Hips")
      {
        vec3 translation = channels[j].get_translation(i);
        channels[j].get_translation(i) = nodeTranslation = vec3(0, translation.y, 0);
      
        mat4 m = toMat4(rotation);
        float x, y, z;
        glm::extractEulerAngleXYZ(m, x, y, z);
        m = glm::eulerAngleXYZ(0.f, y, z);
        hipsRotation[i] =  rotation;
        hipsTranslation[i] = translation;
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

      if (node.name == "Hips")
      {
      }
    }    
  }
  ground_calculate();
}

vec3 AnimationClip::get_root_traslation(uint i) const
{
  i = i < duration ? i : duration - 1;
  return vec3(hipsTranslation[i].x, 0, hipsTranslation[i].z);
}
float AnimationClip::get_root_rotation(uint i) const
{
  i = i < duration ? i : duration - 1;
  mat4 m = toMat4(hipsRotation[i]);
  float x, y, z;
  glm::extractEulerAngleXYZ(m, x, y, z);
  return x;
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
  
  frame.rootRotationDelta = get_root_rotation(i + 1) - get_root_rotation(i);
  frame.rootTranslationDelta = get_root_traslation(i + 1) - get_root_traslation(i);
  return frame;
}

AnimationTrajectory AnimationClip::get_frame_trajectory(uint frame) const
{
  AnimationTrajectory pathFeature;
  vec3 point0 = -hipsTranslation[frame];
  point0.y = 0;
  quat rotation0 = inverse(hipsRotation[frame]);
  for (uint j = 0; j < AnimationTrajectory::PathLength; j++)
  {
    uint next = frame + (uint)(AnimationTrajectory::timeDelays[j] * ticksPerSecond);
    if (!loopable)
    {
      next = next < duration ? next : duration - 1;
      pathFeature.trajectory[j].point = hipsTranslation[next]+point0;
      pathFeature.trajectory[j].rotation = hipsRotation[next]*rotation0;
    }
    else
    {
      if (next < duration)
      {
        pathFeature.trajectory[j].point = hipsTranslation[next]+point0;
        pathFeature.trajectory[j].rotation = hipsRotation[next]*rotation0;
      }
      else
      {
        next -= duration;
        pathFeature.trajectory[j].point = hipsTranslation[frame] - hipsTranslation[0] + hipsTranslation[duration - 1]+point0;
        pathFeature.trajectory[j].rotation = hipsRotation[frame]*inverse(hipsRotation[0])*hipsRotation[duration - 1]*rotation0;
      }
      next = next < duration ? next : duration - 1;
    }
    pathFeature.trajectory[j].timeDelay = AnimationTrajectory::timeDelays[j];
  }
  return pathFeature;
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
  size += write(os, hipsTranslation);
  size += write(os, hipsRotation);
  size += write(os, features);
  size += write(os, tags);
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
  size += read(is, hipsTranslation);
  size += read(is, hipsRotation);
  size += read(is, features);
  size += read(is, tags);
  ground_calculate();
  return size;
}
