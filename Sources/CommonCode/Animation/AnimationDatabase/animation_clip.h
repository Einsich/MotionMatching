#pragma once
#include "../../common.h"
#include "../../Serialization/serialization.h"
#include "../AnimationTree/animation_tree_data.h"
#include "animation_feature.h"
#include "animation_cadr.h"
#include "animation_channel.h"

class AnimationClip: public ISerializable
{
private:
  void ground_calculate();
  void leg_process(int leg_index, u8 leg);
  vector<AnimationChannel> channels;
  vector<vec3> rootTranslationDelta;
  vector<float> rootRotationDelta;
  uint hipsChannelIndex;
public:
  AnimationClip(){}
  AnimationClip(uint duration, float ticksPerSecond, const string &name, AnimationTreeData& tree,
  map<string, vector<quat>> & quats, map<string, vector<vec3>> & vecs);
  uint duration;
  float ticksPerSecond;
  string name;
  vector<AnimationTag> tags;
  vector<AnimationFeatures> features;
  vector<u8> onGround;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  AnimationCadr get_frame(uint frame) const;
  bool contains_tag(AnimationTag tag) const;
};