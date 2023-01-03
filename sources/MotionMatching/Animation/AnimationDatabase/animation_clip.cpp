#include "animation_clip.h"
#include "animation_nodes.h"

AnimationClip::AnimationClip(uint duration, float ticksPerSecond, const std::string &name,
 const AnimationTreeData& tree, std::map<std::string, std::vector<quat>>& quats, 
 std::map<std::string, std::vector<vec3>>& vecs, AnimationTags tags,
 bool loopable, std::string nextClip, bool rotatable):
 hipsTranslation(duration), 
 hipsVelocity(duration), 
 hipsRotation(duration),
 hipsAngularVelocity(duration),
 duration(duration), ticksPerSecond(ticksPerSecond), 
 name(name), tags(tags), 
 loopable(loopable),
 nextClip(nextClip),
 nextClipIdx(-1),
 rotatable(rotatable),features(duration)
{
  debug_log("Animation %s was added, duration %d", name.c_str(), duration);

  std::string hips = "Hips";
  for (uint i = 0; i < tree.nodes.size(); i++)
  {
    const std::string & nodeName = tree.nodes[i].name;
    auto itVec = vecs.find(nodeName);
    auto itQuat = quats.find(nodeName);
    std::vector<vec3> zeroVec = {vec3(0.f)};
    std::vector<quat> zeroQuat = {quat(1,0,0,0)};
    if (nodeName == hips)
      hipsChannelIndex = i;
    //else itVec = vecs.end(), itQuat = quats.end();
    channels.emplace_back(duration, itVec == vecs.end() ? zeroVec : itVec->second, itQuat == quats.end() ? zeroQuat : itQuat->second);

  }
  bool idle = tags.contains("Idle");
  bool crouch = tags.contains("Crouch");
  std::vector<mat4> transfroms(tree.nodes.size());
  for (uint i = 0; i < duration; i++)
  {
    for (uint j = 0; j < tree.nodes.size(); j++)
    {
      const AnimationNodeData& node = tree.nodes[j];
      quat rotation = channels[j].get_rotation(i);
      vec3 nodeTranslation = node.translation;
      if (node.name == hips)
      {
        vec3 translation = channels[j].get_translation(i);
        if (idle && !crouch)
        {
          hipsTranslation[i] = vec3(0);
          nodeTranslation = translation;
        }
        else
        {
          hipsTranslation[i] = vec3(translation.x, 0, translation.z);
          nodeTranslation = vec3(0, translation.y, 0);
        }
        channels[j].get_translation(i) = nodeTranslation;
      
        mat4 m = toMat4(rotation);
        float x, y, z;
        glm::extractEulerAngleXYZ(m, x, y, z);
        m = glm::eulerAngleXYZ(0.f, y, z);
        hipsRotation[i] = idle ? 0 : x;
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
      features[i].features.set_feature(tree.nodes[j].name, nodeTransform[3]);
    }    
  }
  for (uint i = 0; i < duration; i++)
  {
    hipsVelocity[i] = i+1 < duration ?
        (hipsTranslation[i+1] - hipsTranslation[i]) * ticksPerSecond
        : hipsVelocity[i-1];
    if (i+1 < duration)
    {
      float dr = hipsRotation[i+1] - hipsRotation[i];
      dr -= (int)(dr / PITWO)*PITWO;
      float r = abs(dr);
      r = r > PI ? PITWO - r : r;
      hipsAngularVelocity[i] = r * sign(dr) * ticksPerSecond;
    }
    else
      hipsAngularVelocity[i] = hipsAngularVelocity[i-1];
  }

  for (uint i = 0; i < duration; i++)
  {
    int j = i == duration - 1 ? i - 1: i;
    for (int node = 0; node < (int)AnimationFeaturesNode::Count; node++)
    {
      features[i].features.nodesVelocity[node] =
        (features[j + 1].features.nodes[node] - features[j].features.nodes[node]) * ticksPerSecond;
    }
  }
  for (uint i = 0; i < duration; i++)
  {
    features[i].trajectory = get_frame_trajectory(i);
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
  return hipsRotation[i];
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
  
  quat q0 = quat(vec3(0, -get_root_rotation(i), 0));
  frame.rootAngularVelocity = hipsAngularVelocity[i];
  frame.rootLinearVelocity = q0 * hipsVelocity[i];
  return frame;
}

AnimationTrajectory AnimationClip::get_frame_trajectory(uint frame) const
{
  AnimationTrajectory pathFeature;
  vec3 point0 = -hipsTranslation[frame];
  point0.y = 0;
  float rotation1 = hipsRotation[frame];
  float rotation0 = hipsRotation[0];
  quat q0 = quat(vec3(0, -rotation0+hipsRotation[duration-1]-rotation1, 0));
  quat q1 = quat(vec3(0, -rotation1, 0));
  for (uint j = 0; j < AnimationTrajectory::PathLength; j++)
  {
    uint next = frame + (uint)(AnimationTrajectory::timeDelays[j] * ticksPerSecond);
    uint nextM = next % duration;
    pathFeature.trajectory[j].velocity = q1*hipsVelocity[nextM];
    pathFeature.trajectory[j].angularVelocity = hipsAngularVelocity[nextM];
    if (loopable)
    {
      if (next < duration)
      {
        pathFeature.trajectory[j].point = q1*(hipsTranslation[next]+point0);
      }
      else
      {

        int times = next / (duration);
        next -= (times) * (duration);
        times -= 1;
        pathFeature.trajectory[j].point = q0*(hipsTranslation[next] - hipsTranslation[0] + (float)times*(hipsTranslation[duration-1] - hipsTranslation[0])) + q1*(hipsTranslation[duration - 1]+point0);
      }
    }
    else
    {
      if (next < duration)
      {
        pathFeature.trajectory[j].point = q1*(hipsTranslation[next]+point0);
      }
      else
      {
        float times = next - duration + 1;
        vec3 dt = hipsTranslation[duration - 1] - hipsTranslation[duration - 2];
        pathFeature.trajectory[j].point = q1*(dt * times + hipsTranslation[duration - 1]+point0);
      }
    }
    
  }
  return pathFeature;
}

void AnimationClip::leg_process(int leg_index, u8 leg)
{
  std::vector<float> h(features.size());
  std::vector<int> g(features.size());
  for (uint i = 0; i < h.size(); i++)
    h[i] = features[i].features.nodes[leg_index].y;

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
  size += write(os, hipsVelocity);
  size += write(os, hipsRotation);
  size += write(os, hipsAngularVelocity);
  size += write(os, features);
  size += write(os, tags);
  size += write(os, loopable);
  size += write(os, nextClip);
  size += write(os, nextClipIdx);
  size += write(os, rotatable);
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
  size += read(is, hipsVelocity);
  size += read(is, hipsRotation);
  size += read(is, hipsAngularVelocity);
  size += read(is, features);
  size += read(is, tags);
  size += read(is, loopable);
  size += read(is, nextClip);
  size += read(is, nextClipIdx);
  size += read(is, rotatable);
  ground_calculate();
  return size;
}
