#include "animation_database.h"

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
 duration(duration), ticksPerSecond(ticksPerSecond), name(name), cadres(duration), features(duration)
{
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
        nodeTransform = tree.nodes[tree.nodes[j].parent].transform * nodeTransform;
      features[i].set_feature(tree.nodes[j].name, nodeTransform[3]);
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

}
AnimationCadr AnimationClip::get_lerped_cadr(int cadr, float t) const
{
  return cadr < (int)cadres.size() + 1 ? lerped_cadr(cadres[cadr], cadres[cadr + 1], t) : cadres[cadr];
}
size_t AnimationClip::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, cadres);
  size += write(os, features);
  return size;
}
size_t AnimationClip::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, cadres);
  size += read(is, features);
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
  size += write(os, tree);
  return size;
}
size_t AnimationDataBase::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, clips);
  size += read(is, tree);
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

