#include "vp_tree.h"
#include <algorithm>
template<typename It>
static float sort_with_metric(It begin, It end, const std::function<float(const VPTree::T&, const VPTree::T&)> &norma)
{
  size_t dist = end - begin;
  if (dist == 1)
    return 0;
  const VPTree::T *root = (begin + dist/2)->p;
  std::sort(begin, end, [&](const VPTree::Node &a, const VPTree::Node &b){return norma(*root, *a.p) < norma(*root, *b.p);});
  float maxDist = norma(*(end - 1)->p, *begin->p);
  size_t count = dist - 1;
  size_t leftCount = (count + 1) >> 1u;
  float distToLeft = norma(*(begin + leftCount)->p, *begin->p);
  float distToRight = leftCount < count ? norma(*(begin + leftCount+1)->p, *begin->p) : distToLeft + 1.f;
  begin->bound = (distToLeft + distToRight) * 0.5f;
  ++begin;
  sort_with_metric(begin, begin + leftCount,  norma);
  if (leftCount < count)
    sort_with_metric(begin + leftCount, end, norma);
  return maxDist;
}

VPTree::VPTree(AnimationTags tag, std::vector<Node> &&points, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(std::move(points)), norma(norma)
{
  maxRadius = sort_with_metric(this->points.begin(), this->points.end(), norma);
  debug_log("point = %d tags = %d max dist = %f", points.size(), tag.tags, maxRadius);
}

template<typename It>
static bool find_closect(It begin, It end, It &out, int &counter, const VPTree::T &point, float &search_radius,
    const std::function<float(const VPTree::T&, const VPTree::T&)> &norma)
{
  size_t dist = end - begin;
  float rootNorma = norma(*begin->p, point);
  bool hasRoot = rootNorma < search_radius;
  counter++;
  if (hasRoot)
  {
    out = begin;
    search_radius = rootNorma;
  }
  if (dist == 1)
    return hasRoot;

  size_t count = dist - 1;
  size_t leftCount = (count + 1) >> 1u;
  It leftBegin = begin + 1;
  bool childSearch = false;

  if (rootNorma + search_radius < begin->bound || leftCount == count)
  {
    childSearch = find_closect(leftBegin, leftBegin + leftCount, out, counter, point, search_radius, norma);
  }
  else
  {
    if (begin->bound + search_radius < rootNorma)
    {
      childSearch = find_closect(leftBegin + leftCount, end, out, counter, point, search_radius, norma);
    }
    else
    {
      if (!find_closect(leftBegin, leftBegin + leftCount, out, counter, point, search_radius, norma))
      {
        childSearch = find_closect(leftBegin + leftCount, end, out, counter, point, search_radius, norma);
      }
      else
      {
        //if (begin->bound + search_radius < rootNorma)
        {
          find_closect(leftBegin + leftCount, end, out, counter, point, search_radius, norma); 
        }
        childSearch = true;
      }
    }
  }
  return hasRoot || childSearch;
}

std::pair<uint, uint> VPTree::find_closect(const T &point) const
{
  float searchRadius = 100000.f;//glm::max(maxRadius, 100.f);
  auto out = points.cend();
  int counter = 0;
  static uint64_t count = 0, sum = 0, sumX = 0;
  count++;
  if (::find_closect(points.cbegin(), points.cend(), out, counter, point, searchRadius, norma))
  {
    sum += counter;
    sumX += points.size();
    if ((count & (count - 1)) == 0)
      debug_log("average perf %f",  (double)sum / sumX);
    return {out->clip, out->frame};
  }
  
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
  debug_log("vp tree solution  = %f [%d]", searchRadius, out - points.cbegin());
  debug_log("brute force solution = %f [%d]", norma(*points[bestInd].p, point), bestInd);

  return {points[bestInd].clip, points[bestInd].frame};
}