#include "animation_ik.h"
#include "CommonCode/common.h"
#include <glm/gtx/quaternion.hpp>
#include "CommonCode/Components/DebugTools/debug_arrow.h"

#include <vector>
struct Joint
{
  int node;
  mat4 worldTransform;
  mat3 worldRotation;
  vec3 worldPosition;
  vec3 worldDir;
  float length;
};

void process_IK(AnimationTree &tree, AnimationCadr &cadr, const mat4 &toWorld, vec3 target_point, vec3 target_normal, int node,  int node_static_node)
{
  vector<Joint> joints;
  mat4 toRoot = mat4(1.f);
  for (int i = node_static_node; i >= 0; i = tree.nodes[i].parent())
  {
    toRoot = tree.nodes[i].get_transform() * toRoot;
  }
  for (int i = node; i!= node_static_node; i = tree.nodes[i].parent())
  {
    joints.push_back(Joint());
    Joint & joint = joints.back();
    joint.node = i;
  }
  std::reverse(joints.begin(), joints.end());
  for (uint i = 0; i < joints.size(); i++)
  {
    mat4 parent = i == 0 ? toRoot : joints[i - 1].worldTransform;
    joints[i].worldTransform = parent * tree.nodes[joints[i].node].get_transform();
    joints[i].worldRotation = mat3(joints[i].worldTransform);
    joints[i].worldPosition = joints[i].worldTransform[3];
  }
  for (uint i = 0; i < joints.size() - 1; i++)
  {
    joints[i].worldDir = joints[i + 1].worldPosition - joints[i].worldPosition;
    joints[i].length = length(joints[i].worldDir);
    joints[i].worldDir /= joints[i].length;
  }

  constexpr int iterations = 10;
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
      joints[i].worldRotation = toMat3(q) * joints[i].worldRotation;
      joints[i].worldPosition += v * (l - joints[i].length);
      joints[i].worldDir = v;
    }
    joints[0].worldPosition = corner;
    for (int i = 0; i < joints.size() - 1; i++)
    {
      vec3 u = -joints[i].worldDir;
      vec3 d = joints[i].worldPosition - joints[i + 1].worldPosition;
      float l = length(d);
      vec3 v = d / l;
      joints[i].worldRotation = toMat3(quat(-u, -v)) * joints[i].worldRotation ;
      joints[i + 1].worldPosition += v * (l - joints[i].length);
      joints[i].worldDir = -v;
    }
  }

  for (uint i = 0; i < joints.size(); i++)
  {
    joints[i].worldTransform = translate(mat4(1.f), joints[i].worldPosition) * mat4(joints[i].worldRotation);
  }
  for (uint i = 0; i < joints.size(); i++)
  {
    int n = joints[i].node;
    mat4 parent_inv = i > 0 ? inverse(joints[i - 1].worldTransform) : inverse(toRoot);
    mat4 localTransform = parent_inv * joints[i].worldTransform;
    tree.nodes[n].translation = translate(mat4(1.f), vec3(localTransform[3]));
    tree.nodes[n].rotation = mat4(mat3(localTransform));
  }
  int j = joints.size() - 2;
  int k = joints[j].node;
  mat3 toBone = inverse(mat3(joints[j].worldTransform));
  mat4 rot = toMat4(quat(toBone * vec3(0, 1, 0), toBone * target_normal));

  //tree.nodes[k].rotation *= rot;
  constexpr bool draw_arrows = false;
  if (draw_arrows)
  {
    for (uint i = 0; i < joints.size(); i++)
    {
      const mat4 &n = joints[i].worldTransform;
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