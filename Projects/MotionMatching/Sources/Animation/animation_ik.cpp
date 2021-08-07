#include "animation_ik.h"
#include <common.h>
#include <3dmath.h>
#include <Engine/Render/debug_arrow.h>

#include <vector>
struct Joint
{
  int node;
  quat worldRotation;
  vec3 worldPosition;
  vec3 worldDir;
  float length;
};

void process_IK(AnimationTree &tree, AnimationCadr &cadr, const mat4 &toWorld, vec3 target_point, vec3 target_normal, int node,  int node_static_node)
{
  vector<Joint> joints;
  mat4 toRoot = mat4(1.f);
  vec3 rootPos = vec3(0.f);
  quat rootRot = quat(1,0,0,0);
  AnimationTreeData originalTree = tree.get_original_tree();
  int hipsID = originalTree.get_child("Hips");
  for (int i = node_static_node; i >= 0; i = tree.nodes[i].parent())
  {
    if (cadr.nodeRotation[i] == quat())
      cadr.nodeRotation[i] = quat(1,0,0,0);
    rootRot = cadr.nodeRotation[i] * rootRot;
    rootPos = (hipsID == i ? cadr.nodeTranslation : originalTree.nodes[i].translation) + cadr.nodeRotation[i] * rootPos;

  }

    toRoot = translate(mat4(1.f),rootPos) * toMat4(rootRot);

  for (int i = node; i!= node_static_node; i = tree.nodes[i].parent())
  {
    joints.push_back(Joint());
    Joint & joint = joints.back();
    joint.node = i;
  }
  std::reverse(joints.begin(), joints.end());
  for (uint i = 0; i < joints.size(); i++)
  {
    vec3 parentPos = i == 0 ? rootPos : joints[i - 1].worldPosition;
    quat parentRot = i == 0 ? rootRot : joints[i - 1].worldRotation;
    int k = joints[i].node;
    joints[i].worldRotation = parentRot * cadr.nodeRotation[k];
    joints[i].worldPosition = parentPos + parentRot * originalTree.nodes[k].translation;

  }
  for (int i = 0; i < (int)joints.size() - 1; i++)
  {
    joints[i].worldDir = joints[i + 1].worldPosition - joints[i].worldPosition;
    joints[i].length = length(joints[i].worldDir);
    joints[i].worldDir /= joints[i].length;
  }

  constexpr int iterations = 4;
  vec3 corner = joints[0].worldPosition;
  for (int k = 0; k < iterations; k++)
  {
    int last = joints.size() - 1;
    float dist = length2(vec3(joints[last].worldPosition) - target_point);
    if (dist < 0.0001f)
      break;
    joints[last].worldPosition = target_point;
    for (int i = last - 1; i >=0; i--)
    {
      vec3 u = joints[i].worldDir;
      vec3 d = joints[i + 1].worldPosition - joints[i].worldPosition;
      float l = length(d);
      vec3 v = d / l;
      quat q = quat(u, v);
      joints[i].worldRotation = joints[i].worldRotation * q;
      joints[i].worldPosition += v * (l - joints[i].length);
      joints[i].worldDir = v;
    }
    joints[0].worldPosition = corner;
    for (int i = 0; i < (int)joints.size() - 1; i++)
    {
      vec3 u = -joints[i].worldDir;
      vec3 d = joints[i].worldPosition - joints[i + 1].worldPosition;
      float l = length(d);
      vec3 v = d / l;
      joints[i].worldRotation = joints[i].worldRotation * quat(-u, -v) ;
      joints[i + 1].worldPosition += v * (l - joints[i].length);
      joints[i].worldDir = -v;
    }
  }

  vector<mat4> worldTransform(joints.size());
  for (uint i = 0; i < joints.size(); i++)
  {
    worldTransform[i] = translate(mat4(1.f), joints[i].worldPosition) * mat4(joints[i].worldRotation);
  }
  for (uint i = 0; i < joints.size(); i++)
  {
    int n = joints[i].node;
    
    quat q = joints[i].worldRotation;
      q = inverse(i != 0 ? joints[i - 1].worldRotation : rootRot)  * q;
    cadr.nodeRotation[n] = q;
  }
  int j = joints.size() - 1;
  int k = joints[j].node;
  mat3 toBone = inverse(mat3(worldTransform[j]));
  quat q = quat(toBone * vec3(0, 1, 0), toBone * target_normal);
  cadr.nodeRotation[k] = q * cadr.nodeRotation[k];
  constexpr bool draw_arrows = false;
  if (draw_arrows)
  {
    for (uint i = 0; i < joints.size(); i++)
    {
      const mat4 &n = worldTransform[i];
      vec3 p = toWorld * vec4(vec3(n[3]), 1);
      vec3 d[3] = {vec3(1,0,0), vec3(0,1,0), vec3(0,0,1)};
      for (int j = 0; j < 3; j ++)
      {
        vec3 dp = toWorld * vec4(vec3(n * vec4(d[j]* 0.3f, 0)), 0);
        draw_arrow(p, p + dp, d[j], 0.02f, true);
      }
    }
    draw_arrow(toWorld * vec4(joints[j + 1].worldPosition, 1), toWorld * vec4(target_point, 1), vec3(1.f), 0.02f, true);
  }

}