#include "animation_database.h"
static map<string, vector<AnimationTag>> tagMap = {
{"MOB1_Crouch_To_Stand_Relaxed", {AnimationTag::Stay}},
{"MOB1_Stand_Rlx_Turn_In_Place_L_Loop", {AnimationTag::Stay}},
{"MOB1_Crouch_R_90", {AnimationTag::Crouch}},
{"MOB1_Walk_F_Loop", {AnimationTag::Stay}},
{"MOB1_Walk_F_Jump_RU", {AnimationTag::Jump}},
{"MOB1_Stand_Relaxed_R_90", {AnimationTag::Stay}},
{"MOB1_Jog_F_Loop", {AnimationTag::Stay}},
{"MOB1_Crouch_L_90", {AnimationTag::Crouch}},
{"MOB1_Crouch_Idle_V2", {AnimationTag::Crouch}},
{"MOB1_Stand_Relaxed_To_Jog_F", {AnimationTag::Stay}},
{"MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", {AnimationTag::Crouch}},
{"MOB1_Crouch_To_CrouchWalk_F", {AnimationTag::Crouch}},
{"MOB1_CrouchWalk_F_To_Crouch_RU", {AnimationTag::Crouch}},
{"MOB1_Stand_Relaxed_To_Crouch", {AnimationTag::Crouch}},
{"MOB1_Walk_F_Jump", {AnimationTag::Jump}},
{"MOB1_Stand_Rlx_Turn_In_Place_R_Loop", {AnimationTag::Stay}},
{"MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", {AnimationTag::Crouch}},
{"MOB1_Stand_Relaxed_Idle_v2", {AnimationTag::Stay}},
{"MOB1_Walk_F_To_Stand_Relaxed_RU", {AnimationTag::Stay}},
{"MOB1_Jog_F_Jump_RU", {AnimationTag::Jump}},
{"MOB1_Jog_F_Jump", {AnimationTag::Jump}},
{"MOB1_Stand_Relaxed_L_90", {AnimationTag::Stay}},
{"MOB1_Stand_Relaxed_To_Walk_F", {AnimationTag::Stay}},
{"MOB1_CrouchWalk_F_Loop", {AnimationTag::Crouch}}
};
vec3 get_vec3(const map<string, vector<vec3>>& vecs, const string& name, uint i)
{
  auto it = vecs.find(name);
  return (it == vecs.cend() || i < 0 || it->second.size() <= i) ? vec3(0.f) : it->second[i];
}
quat get_quat(const map<string, vector<quat>>& vecs, const string& name, uint i)
{
  auto it = vecs.find(name);
  return (it == vecs.cend() || i < 0 || it->second.size() <= i) ? quat() : it->second[i];
}

AnimationClip::AnimationClip(uint duration, float ticksPerSecond, const string &name,
 AnimationTreeData& tree, const map<string, vector<quat>>& quats, const map<string, vector<vec3>>& vecs):
 duration(duration), ticksPerSecond(ticksPerSecond), name(name), tags(tagMap[name]), cadres(duration), features(duration)
{
  vector<mat4> transfroms(tree.nodes.size());
  for (uint i = 0; i < duration; i++)
  {
    AnimationCadr& cadr = cadres[i];
    cadr.nodeRotation.resize(tree.nodes.size());
    for (uint j = 0; j < tree.nodes.size(); j++)
    {
      const AnimationNodeData& node = tree.nodes[j];
      quat rotation = get_quat(quats, node.name, i);
      if (node.name == "Hips")
      {
        vec3 translation = get_vec3(vecs, node.name, i);
        cadr.rootTranslationDelta = vec3(translation.x, 0, translation.z);
        cadr.nodeTranslation = vec3(0, translation.y, 0);
      
        mat4 m = toMat4(rotation);
        float x, y, z;
        glm::extractEulerAngleXYZ(m, x, y, z);
        m = glm::eulerAngleXYZ(0.f, y, z);
        cadr.rootRotationDelta = x;
        rotation = quat_cast(m);
      }
      cadr.nodeRotation[j] = rotation;
      mat4 nodeTransform = tree.nodes[j].transform;
      mat4 rotationM = glm::toMat4(rotation);
      mat4 translation = (node.name == "Hips") ? glm::translate(mat4(1.f), cadr.nodeTranslation) * mat4(mat3(nodeTransform)) : nodeTransform;
      nodeTransform = translation * rotationM;
      if (tree.nodes[j].parent >= 0)
        nodeTransform = transfroms[tree.nodes[j].parent] * nodeTransform;
      transfroms[j] = nodeTransform;
      features[i].set_feature(tree.nodes[j].name, nodeTransform[3]);
    }    
  }
  ground_calculate();
  vec3 lastRootPos = cadres[0].rootTranslationDelta;
  for (uint i = 1; i < duration; i++)
  {
    if ((onGround[i-1] & 1) && (onGround[i] & 1) && (onGround[i-1] & 2) && (onGround[i] & 2))
    {
      vec3 d = cadres[i].rootTranslationDelta - lastRootPos;
      cadres[i].rootTranslationDelta -= d;
      cadres[i].nodeTranslation += d;
      for(vec3 &feature : features[i].features)
        feature += d;
    } else
    {
      lastRootPos = cadres[i].rootTranslationDelta;
    }
  }
  for (int i = duration - 1; i >=0; i--)
  {
    AnimationCadr& cadr1 = cadres[i];
    AnimationCadr& cadr2 = cadres[i - 1];
    cadr1.rootRotationDelta -= cadr2.rootRotationDelta;
    cadr1.rootTranslationDelta -= cadr2.rootTranslationDelta;
  }
  cadres[0].rootRotationDelta = 0;
  cadres[0].rootTranslationDelta = vec3(0.f);

  for (uint i = 0; i < duration; i++)
  {
    vec3 dt = cadres[i].rootTranslationDelta;
    float dr = cadres[i].rootRotationDelta;
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
        if (nextInd < cadres.size())
        {
          dt = cadres[nextInd].rootTranslationDelta;
          dr = cadres[nextInd].rootRotationDelta;
        }
        pathFeature.path[j] += dt;
        pathFeature.rotation += dr;
      }
    }
  }
}
AnimationCadr AnimationClip::get_lerped_cadr(int cadr, float t) const
{
  return cadr < (int)cadres.size() + 1 ? lerped_cadr(cadres[cadr], cadres[cadr + 1], t) : cadres[cadr];
}
void AnimationClip::ground_calculate()
{
  onGround.resize(features.size(), 0);
  vector<float> h(features.size());
  for (uint i = 0; i < h.size(); i++)
    h[i] = features[i].features[(int)AnimationFeaturesNode::LeftToeBase].y;
  for (uint i = 0; i < h.size(); i++)
    if(abs(h[i]) < 0.05f)
      onGround[i] |= 1;
  for (uint i = 0; i < h.size(); i++)
    h[i] = features[i].features[(int)AnimationFeaturesNode::RightToeBase].y;
  for (uint i = 0; i < h.size(); i++)
    if(abs(h[i]) < 0.05f)
      onGround[i] |= 2;
}
size_t AnimationClip::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, duration);
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, cadres);
  size += write(os, features);
  return size;
}
size_t AnimationClip::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, duration);
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, cadres);
  size += read(is, features);
  tags = tagMap[name];
  ground_calculate();
  return size;
}


AnimationDataBase::AnimationDataBase(aiNode *root):
tree(root), featureWeights(make_shared<AnimationFeaturesWeights>()){}
int AnimationDataBase::cadr_count() const
{
  int count = 0;
  for (const AnimationClip & anim: clips)
    count += anim.duration;
  return count;
}
size_t AnimationDataBase::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, clips);
  return size;
}
size_t AnimationDataBase::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, clips);
  return size;
}
void AnimationDataBase::save_runtime_parameters()
{
  save_object(*featureWeights, "FeaturesWeights.bin");
}
void AnimationDataBase::load_runtime_parameters()
{
  load_object(*featureWeights, "FeaturesWeights.bin");
}

