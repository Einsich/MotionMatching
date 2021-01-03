#include "animation.h"
#include "CommonCode/Time/time.h"
#include "CommonCode/Light/direction_light.h"
void AnimationPlayer::CalculateBonesTransform(AnimationNode &node, mat4 parent, int d)
{
  mat4 nodeTransform = node.transform;
  auto it = animations[curAnim].channels.find(node.name);
  if (it != animations[curAnim].channels.end())
  {
    AnimationChannel& channel = it->second;
    mat4 rotation =  channel.get_lerped_rotation(curCadr, curT);
    
    if (d == 0)
    {
      mat4 translation = channel.get_lerped_translation(curCadr, curT);
      nodeTransform = translation * nodeTransform * rotation;
      gameObject->get_transform().set_rotation(mat4(mat3(nodeTransform)));

      gameObject->get_transform().set_position(nodeTransform[3]);
      parent = nodeTransform = mat4(1.f);
        //BoneRender.rootPoint = new Vector3D(nodeTransform.A4, nodeTransform.B4, nodeTransform.C4);
    }   
    else
    {
      nodeTransform = nodeTransform * rotation;
    }
                      
  }
  else
  {
      //Console.WriteLine($"!{node.Name} + {(animNode != null ? animNode.Name : "null")}");
  }
  node.animationTransform = nodeTransform;
  nodeTransform = parent * nodeTransform;
  //if (d == 0)
  //    BoneRender.rootPoint = new Vector3D(nodeTransform.A4, nodeTransform.B4, nodeTransform.C4);
  //BoneRender.AddBones(parent, nodeTransform, new Vector3D(0.1f,1.5f,0.1f), node.Name);

  auto it2 = gameObject->get_mesh()->bonesMap.find(node.name);
  if (it2 != gameObject->get_mesh()->bonesMap.end())
  {
    curTransform[it2->second] = nodeTransform * node.meshToBone;
  }
  for (uint i = 0; i < node.childs.size(); i++)
  {
    CalculateBonesTransform(animationTree.nodes[node.childs[i]], nodeTransform, d + 1);
  }
}
void AnimationPlayer::PlayNextCadr()
{
  
  CalculateBonesTransform(animationTree.nodes[0], mat4(1.f), 0);
  curT += speed * Time::delta_time() * animations[curAnim].ticksPerSecond;
  int skippedCardes = (int)curT;
  curT -= skippedCardes;
  curCadr += skippedCardes;
  if (curCadr >= animations[curAnim].duration - 1)
  {
    curCadr = 0;
    curT = 0;
    curAnim++;
    if (curAnim >= animations.size())
    {
      curAnim = 0;
    }
    log_debug("play %s", animations[curAnim].name.c_str());
  }
}
int AnimationPlayer::cadr_count()
{
  int count = 0;
  for (Animation & anim: animations)
    count += anim.duration;
  return count;
}
void AnimationPlayer::render(const Camera& mainCam, const DirectionLight& light)
{
  gameObject->get_shader().use();
  gameObject->get_shader().set_mat4x4("Bones", curTransform, false);
  gameObject->render(mainCam, light);
  boneRender.render(gameObject->get_transform().get_transform(), animationTree, mainCam, light);
}
size_t Animation::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, duration);
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, channels);
  return size;
}
size_t Animation::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, duration);
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, channels);
  return size;
}
size_t AnimationPlayer::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, animationTree);
  size += write(os, animations);
  size += write(os, curTransform.size());
  return size;
}
size_t AnimationPlayer::deserialize(std::istream& is)
{
  size_t size = 0;
  size_t nodeCount;
  size += read(is, animationTree);
  size += read(is, animations);
  size += read(is, nodeCount);
  curTransform.resize(nodeCount);
  return size;
}