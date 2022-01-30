#pragma once
#include <set>
#include <common.h>
#include "serialization/serialization.h"
#include "../AnimationTree/animation_tree_data.h"
#include "animation_feature.h"
#include "animation_cadr.h"
#include "animation_channel.h"

class AnimationClip final : public ISerializable
{
private:
  void ground_calculate();
  void leg_process(int leg_index, u8 leg);
  vector<AnimationChannel> channels;
  vector<vec3> hipsTranslation, hipsVelocity;
  vector<float> hipsRotation, hipsAngularVelocity;


  uint hipsChannelIndex;
  vec3 get_root_traslation(uint i) const;
  float get_root_rotation(uint i) const;
public:
  AnimationClip()=default;
  ~AnimationClip()=default;
  AnimationClip(uint duration, float ticksPerSecond, const string &name, const AnimationTreeData& tree,
  map<string, vector<quat>> & quats, map<string, vector<vec3>> & vecs, AnimationTags tags,
  bool loopable, string nextClip, bool rotatable);
  uint duration;
  float ticksPerSecond;
  string name;
  AnimationTags tags;
  bool loopable;
  string nextClip;
  int nextClipIdx;
  bool rotatable;
  vector<FrameFeature> features;

  vector<u8> onGround;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  AnimationCadr get_frame(uint frame) const;
  AnimationTrajectory get_frame_trajectory(uint frame) const;
};