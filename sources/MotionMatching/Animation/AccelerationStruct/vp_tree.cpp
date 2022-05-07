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
  begin->nearestNeighbor = norma(*(begin + 1)->p, *begin->p);
  ++begin;
  sort_with_metric(begin, begin + leftCount,  norma);
  if (leftCount < count)
    sort_with_metric(begin + leftCount, end, norma);
  return maxDist;
}

VPTree::VPTree(AnimationTags tag, std::vector<Node> &&m_points, std::function<float(const T&, const T&)> &&norma):
  tag(tag), points(std::move(m_points)), norma(norma)
{
  maxRadius = sort_with_metric(this->points.begin(), points.end(), norma);
  debug_log("point = %d tags = %s max dist = %f", points.size(), tag.to_string().c_str(), maxRadius);
}

template<typename It>
struct Solver
{
  It begin, end, out;
  const VPTree::T &point;
  float search_radius;
  float tolerance_erorr;
  int counter;
  const std::function<float(const VPTree::T&, const VPTree::T&)> &norma;
  Solver(It begin, It end, const VPTree::T &point,
      float tolerance_erorr,
      const std::function<float(const VPTree::T&, const VPTree::T&)> &norma)
  : begin(begin), end(end), out(end),
   point(point), search_radius(0), tolerance_erorr(tolerance_erorr), counter(0), norma(norma)
  {}

  bool find_closect(It &out, int &counter, float &search_radius)
  {
    this->search_radius = search_radius;
    bool result = find_closect(begin, end);
    out = this->out;
    counter = this->counter;
    search_radius = this->search_radius;
    return result;
  }
private:
  bool find_closect(It begin, It end)
  {
    if (search_radius <= 0)
      return false;
    size_t dist = end - begin;
    float rootNorma = norma(*begin->p, point);
    bool hasRoot = rootNorma < search_radius;
    counter++;
    if (hasRoot)
    {
      out = begin;
      search_radius = rootNorma;
      if (rootNorma < tolerance_erorr)
      {
        search_radius = 0;
        return true;
      }
    }
    if (dist == 1 || rootNorma < begin->nearestNeighbor * 0.5)
      return hasRoot;

    size_t count = dist - 1;
    size_t leftCount = (count + 1) >> 1u;
    It leftBegin = begin + 1;
    bool childSearch = false;

    if (rootNorma + search_radius < begin->bound || leftCount == count)
    {
      childSearch = find_closect(leftBegin, leftBegin + leftCount);
    }
    else
    {
      if (begin->bound + search_radius < rootNorma)
      {
        childSearch = find_closect(leftBegin + leftCount, end);
      }
      else
      {
        bool leftSearch = find_closect(leftBegin, leftBegin + leftCount);
        childSearch = (!leftSearch && find_closect(leftBegin + leftCount, end)) ||
            (leftSearch && rootNorma + search_radius >= begin->bound && find_closect(leftBegin + leftCount, end));
      }
    }
    return hasRoot || childSearch;
  }
};

std::pair<uint, uint> VPTree::find_closect(const T &point, float tolerance_error) const
{
  float searchRadius = 100000.f;//glm::max(maxRadius, 100.f);
  auto out = points.cend();
  int counter = 0;
  static uint64_t count = 0, sum = 0, sumX = 0;
  count++;
  Solver solver(points.cbegin(), points.cend(), point, tolerance_error, norma);
  if (solver.find_closect(out, counter, searchRadius))
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
  float error = glm::abs(searchRadius - norma(*points[bestInd].p, point));
  if (error > 0.1f)
    debug_log("error %f vp tree[%d] brute force[%d]", error, out - points.cbegin(), bestInd);

  return {points[bestInd].clip, points[bestInd].frame};
}