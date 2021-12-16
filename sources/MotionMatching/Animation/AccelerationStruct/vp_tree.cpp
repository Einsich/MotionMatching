#include "vp_tree.h"

VPTree::VPTree(AnimationTags tag, std::vector<Node> &&points, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(points), norma(norma)
{

}

std::pair<uint, uint> VPTree::find_closect(const T &point) const
{
  float minRes = norma(*points[0].p, point);
  uint bestInd = 0;
  for (uint nextFrame = 1, n = points.size(); nextFrame < n; nextFrame++)
  {
    float result = norma(*points[nextFrame].p, point);
    if (result < minRes)
    {
      minRes = result;
      bestInd = nextFrame;
    }
  }
  return {points[bestInd].clip, points[bestInd].frame};
}